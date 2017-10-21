/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                       Copyright (c) 2000-2008                         */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  September 2000                                   */
/*************************************************************************/
/*                                                                       */
/*  Basic user level functions                                           */
/*                                                                       */
/*************************************************************************/

#include "cst_tokenstream.h"
#include "flite.h"
#include "cst_alloc.h"
#include "cst_clunits.h"
#include "cst_cg.h"

#ifdef WIN32
/* For Visual Studio 2012 global variable definitions */
#define GLOBALVARDEF __declspec(dllexport)
#else
#define GLOBALVARDEF
#endif

/* This is a global, which isn't ideal, this may change */
/* It is set when flite_set_voice_list() is called which happens in */
/* flite_main(), but it is now also possible to leave this unset if */
/* all voice selection names are pathnames to (cg) ./flitevox files */
/* then this gets populated as the voices get loaded                */
/* Note these voices remain loaded, there is currently no automatic */
/* garbage collection, that would be necessary in the long run      */
/* delete_voice will work, but you'd need to know when to call it   */
GLOBALVARDEF cst_val *flite_voice_list = NULL;

/* Another global with hold pointers to the language and lexicon    */
/* initalization functions, we limiting to 20 but it could be bigger */
/* if we really did support over 20 different languages             */
#define FLITE_MAX_LANGS 20
GLOBALVARDEF cst_lang flite_lang_list[FLITE_MAX_LANGS];
GLOBALVARDEF int flite_lang_list_length = 0;

int flite_init()
{
    cst_regex_init();

    return 0;
}

int flite_voice_dump(cst_voice *voice, const char *filename)
{
    return cst_cg_dump_voice(voice,filename);
}

cst_voice *flite_voice_load(const char *filename)
{
    /* Currently only supported for CG voices */
    /* filename make be a local pathname or a url (http:/file:) */
    cst_voice *v = NULL;

    v = cst_cg_load_voice(filename,flite_lang_list);

    return v;
}

int flite_add_voice(cst_voice *voice)
{
    const cst_val *x;
    if (voice)
    {
        /* add to second place -- first is default voice */
        /* This is thread unsafe */
        if (flite_voice_list)
        {   /* Other voices -- first is default, add this second */
            x = cons_val(voice_val(voice),
                         val_cdr(flite_voice_list));
            set_cdr((cst_val *)(void *)flite_voice_list,x);
        }
        else
        {   /* Only voice so goes on front */
            flite_voice_list = cons_val(voice_val(voice),flite_voice_list);
        }
        
        return TRUE;
    }
    else
        return FALSE;

}

int flite_add_lang(const char *langname,
                   void (*lang_init)(cst_voice *vox),
                   cst_lexicon *(*lex_init)())
{
    if (flite_lang_list_length < (FLITE_MAX_LANGS-1))
    {
        flite_lang_list[flite_lang_list_length].lang = langname;
        flite_lang_list[flite_lang_list_length].lang_init = lang_init;
        flite_lang_list[flite_lang_list_length].lex_init = lex_init;
        flite_lang_list_length++;
        flite_lang_list[flite_lang_list_length].lang = NULL;
    }

    return TRUE;
}


cst_voice *flite_voice_select(const char *name)
{
    const cst_val *v;
    cst_voice *voice;

    if (name == NULL)
    {
        if (flite_voice_list == NULL)
            return NULL;  /* oops, not good */
        return val_voice(val_car(flite_voice_list));
    }

    for (v=flite_voice_list; v; v=val_cdr(v))
    {
        voice = val_voice(val_car(v));
        if (cst_streq(name,voice->name))  /* short name */
            return voice;
        if (cst_streq(name,get_param_string(voice->features,"name","")))
            /* longer name */
            return voice;
        if (cst_streq(name,get_param_string(voice->features,"pathname","")))
            /* even longer name (url) */
            return voice;
    }

    if (cst_urlp(name) || /* naive check if its a url */
        cst_strchr(name,'/') ||
        cst_strchr(name,'\\') ||
        cst_strstr(name,".flitevox"))
    {
        voice = flite_voice_load(name);
        if (!voice)
            cst_errmsg("Error load voice: failed to load voice from %s\n",name);
        else
            flite_add_voice(voice);
        return voice;
    }

    return flite_voice_select(NULL);

}

int flite_voice_add_lex_addenda(cst_voice *v, const cst_string *lexfile)
{
    /* Add addenda in lexfile to current voice */
    cst_lexicon *lex;
    const cst_val *lex_addenda = NULL;
    cst_val *new_addenda;

    lex = val_lexicon(feat_val(v->features,"lexicon"));
    if (feat_present(v->features, "lex_addenda"))
	lex_addenda = feat_val(v->features, "lex_addenda");

    new_addenda = cst_lex_load_addenda(lex,lexfile);
#if 0
    printf("\naddenda: ");
    val_print(stdout,new_addenda);
    printf("\n");
#endif

    new_addenda = val_append(new_addenda,(cst_val *)lex_addenda);
    if (lex->lex_addenda)
        delete_val(lex->lex_addenda);
    lex->lex_addenda = new_addenda;

    return 0;
}

cst_utterance *flite_do_synth(cst_utterance *u,
                                     cst_voice *voice,
                                     cst_uttfunc synth)
{		       
    utt_init(u, voice);
    if ((*synth)(u) == NULL)
    {
	delete_utterance(u);
	return NULL;
    }
    else
	return u;
}

cst_utterance *flite_synth_text(const char *text, cst_voice *voice)
{
    cst_utterance *u;

    u = new_utterance();
    utt_set_input_text(u,text);
    return flite_do_synth(u, voice, utt_synth);
}

cst_utterance *flite_synth_phones(const char *text, cst_voice *voice)
{
    cst_utterance *u;

    u = new_utterance();
    utt_set_input_text(u,text);
    return flite_do_synth(u, voice, utt_synth_phones);
}

cst_wave *flite_text_to_wave(const char *text, cst_voice *voice)
{
    cst_utterance *u;
    cst_wave *w;

    if ((u = flite_synth_text(text,voice)) == NULL)
	return NULL;

    w = copy_wave(utt_wave(u));
    delete_utterance(u);
    return w;
}

float flite_file_to_speech(const char *filename, 
			   cst_voice *voice,
			   const char *outtype)
{
    cst_tokenstream *ts;

    if ((ts = ts_open(filename,
	      get_param_string(voice->features,"text_whitespace",NULL),
	      get_param_string(voice->features,"text_singlecharsymbols",NULL),
	      get_param_string(voice->features,"text_prepunctuation",NULL),
	      get_param_string(voice->features,"text_postpunctuation",NULL)))
	== NULL)
    {
	cst_errmsg("failed to open file \"%s\" for reading\n",
		   filename);
	return 1;
    }
    return flite_ts_to_speech(ts,voice,outtype);
}


float flite_ts_to_speech(cst_tokenstream *ts,
                         cst_voice *voice,
                         const char *outtype)
{
    cst_utterance *utt;
    const char *token;
    cst_item *t;
    cst_relation *tokrel;
    float durs = 0;
    int num_tokens;
    cst_wave *w;
    cst_breakfunc breakfunc = default_utt_break;
    cst_uttfunc utt_user_callback = 0;
    int fp;

    fp = get_param_int(voice->features,"file_start_position",0);
    if (fp > 0)
        ts_set_stream_pos(ts,fp);
    if (feat_present(voice->features,"utt_break"))
	breakfunc = val_breakfunc(feat_val(voice->features,"utt_break"));

    if (feat_present(voice->features,"utt_user_callback"))
	utt_user_callback = val_uttfunc(feat_val(voice->features,"utt_user_callback"));

    /* If its a file to write to, create and save an empty wave file */
    /* as we are going to incrementally append to it                 */
    if (!cst_streq(outtype,"play") && 
        !cst_streq(outtype,"none") &&
        !cst_streq(outtype,"stream"))
    {
	w = new_wave();
	cst_wave_resize(w,0,1);
	cst_wave_set_sample_rate(w,16000);
	cst_wave_save_riff(w,outtype);  /* an empty wave */
	delete_wave(w);
    }

    num_tokens = 0;
    utt = new_utterance();
    tokrel = utt_relation_create(utt, "Token");
    while (!ts_eof(ts) || num_tokens > 0)
    {
	token = ts_get(ts);
	if ((cst_strlen(token) == 0) ||
	    (num_tokens > 500) ||  /* need an upper bound */
	    (relation_head(tokrel) && 
	     breakfunc(ts,token,tokrel)))
	{
	    /* An end of utt, so synthesize it */
            if (utt_user_callback)
                utt = (utt_user_callback)(utt);

            if (utt)
            {
                utt = flite_do_synth(utt,voice,utt_synth_tokens);
                if (feat_present(utt->features,"Interrupted"))
                {
                    delete_utterance(utt); utt = NULL;
                    break;
                }
                durs += flite_process_output(utt,outtype,TRUE);
                delete_utterance(utt); utt = NULL;
            }
            else 
                break;

	    if (ts_eof(ts)) break;

	    utt = new_utterance();
	    tokrel = utt_relation_create(utt, "Token");
	    num_tokens = 0;
	}
	num_tokens++;

	t = relation_append(tokrel, NULL);
	item_set_string(t,"name",token);
	item_set_string(t,"whitespace",ts->whitespace);
	item_set_string(t,"prepunctuation",ts->prepunctuation);
	item_set_string(t,"punc",ts->postpunctuation);
        /* Mark it at the beginning of the token */
	item_set_int(t,"file_pos",
                     ts->file_pos-(1+ /* as we are already on the next char */
                                   cst_strlen(token)+
                                   cst_strlen(ts->prepunctuation)+
                                   cst_strlen(ts->postpunctuation)));
	item_set_int(t,"line_number",ts->line_number);
    }
    if (utt) delete_utterance(utt);
    ts_close(ts);
    return durs;
}

float flite_text_to_speech(const char *text,
			   cst_voice *voice,
			   const char *outtype)
{
    cst_utterance *u;
    float dur;

    u = flite_synth_text(text,voice);
    dur = flite_process_output(u,outtype,FALSE);
    delete_utterance(u);

    return dur;
}

float flite_phones_to_speech(const char *text,
			     cst_voice *voice,
			     const char *outtype)
{
    cst_utterance *u;
    float dur;

    u = flite_synth_phones(text,voice);
    dur = flite_process_output(u,outtype,FALSE);
    delete_utterance(u);

    return dur;
}

float flite_process_output(cst_utterance *u, const char *outtype,
                           int append)
{
    /* Play or save (append) output to output file */
    cst_wave *w;
    float dur;

    if (!u) return 0.0;

    w = utt_wave(u);

    dur = (float)w->num_samples/(float)w->sample_rate;
	     
    if (cst_streq(outtype,"play"))
	play_wave(w);
    else if (cst_streq(outtype,"stream"))
    {
        /* It's already been played so do nothing */
        
    }
    else if (!cst_streq(outtype,"none"))
    {
        if (append)
            cst_wave_append_riff(w,outtype);
        else
            cst_wave_save_riff(w,outtype);
    }

    return dur;
}

int flite_get_param_int(const cst_features *f, const char *name,int def)
{
    return get_param_int(f,name,def);
}
float flite_get_param_float(const cst_features *f, const char *name, float def)
{
    return get_param_float(f,name,def);
}
const char *flite_get_param_string(const cst_features *f, const char *name, const char *def)
{
    return get_param_string(f,name,def);
}
const cst_val *flite_get_param_val(const cst_features *f, const char *name, cst_val *def)
{
    return get_param_val(f,name,def);
}

void flite_feat_set_int(cst_features *f, const char *name, int v)
{
    feat_set_int(f,name,v);
}
void flite_feat_set_float(cst_features *f, const char *name, float v)
{
    feat_set_float(f,name,v);
}
void flite_feat_set_string(cst_features *f, const char *name, const char *v)
{
    feat_set_string(f,name,v);
}
void flite_feat_set(cst_features *f, const char *name,const cst_val *v)
{
    feat_set(f,name,v);
}
int flite_feat_remove(cst_features *f, const char *name)
{
    return feat_remove(f,name);
}

const char *flite_ffeature_string(const cst_item *item,const char *featpath)
{
    return ffeature_string(item,featpath);
}
int flite_ffeature_int(const cst_item *item,const char *featpath)
{
    return ffeature_int(item,featpath);
}
float flite_ffeature_float(const cst_item *item,const char *featpath)
{
    return ffeature_float(item,featpath);
}
const cst_val *flite_ffeature(const cst_item *item,const char *featpath)
{
    return ffeature(item,featpath);
}

cst_item* flite_path_to_item(const cst_item *item,const char *featpath)
{
    return path_to_item(item,featpath);
}

int flite_mmap_clunit_voxdata(const char *voxdir, cst_voice *voice)
{   
    /* Map clunit_db in voice data for giveb voice */
    char *path;
    const char *name;
    const char *x;
    int *indexes;
    cst_filemap *vd;
    cst_clunit_db *clunit_db;
    int i;

    name = get_param_string(voice->features,"name","voice");
    path = cst_alloc(char,cst_strlen(voxdir)+1+cst_strlen(name)+1+cst_strlen("voxdata")+1);
    cst_sprintf(path,"%s/%s.voxdata",voxdir,name);

    vd = cst_mmap_file(path);
    
    flite_feat_set_string(voice->features,"voxdir",path);
    cst_free(path);

    if (vd == NULL)
        return -1;

    x = (const char *)vd->mem;
    if (!cst_streq("CMUFLITE",x))
    {   /* Not a Flite voice data file */
        cst_munmap_file(vd);
        return -1;
    }

    for (i=9; x[i] &&i<64; i++)
        if (x[i] != ' ')
            break;

    if (!cst_streq(name,&x[i]))
    {   /* Not a voice data file for this voice */
        cst_munmap_file(vd);
        return -1;
    }

    /* This uses a hack to put in a void pointer to the cst_filemap */
    flite_feat_set(voice->features,"voxdata",userdata_val(vd));
    indexes = (int *)&x[64];
    
    clunit_db = val_clunit_db(feat_val(voice->features,"clunit_db"));

    clunit_db->sts->resoffs = 
        (const unsigned int *)&x[64+20];
    clunit_db->sts->frames = 
        (const unsigned short *)&x[64+20+indexes[0]];
    clunit_db->mcep->frames = 
        (const unsigned short *)&x[64+20+indexes[0]+indexes[1]];
    clunit_db->sts->residuals = 
        (const unsigned char *)&x[64+20+indexes[0]+indexes[1]+indexes[2]];
    clunit_db->sts->ressizes = 
        (const unsigned char *)&x[64+20+indexes[0]+indexes[1]+indexes[2]+indexes[3]];
    
    return 0;
}

int flite_munmap_clunit_voxdata(cst_voice *voice)
{

    cst_filemap *vd;
    const cst_val *val_vd;
    const cst_val *val_clunit_database;
    cst_clunit_db *clunit_db;

    val_vd = flite_get_param_val(voice->features,"voxdata",NULL);
    val_clunit_database = flite_get_param_val(voice->features,"clunit_db",NULL);

    if (val_vd && val_clunit_database)
    {    
        clunit_db = val_clunit_db(val_clunit_database);
        clunit_db->sts->resoffs = NULL;
        clunit_db->sts->frames = NULL;
        clunit_db->mcep->frames = NULL;
        clunit_db->sts->residuals = NULL;
        clunit_db->sts->ressizes = NULL;
        vd = (cst_filemap *)val_userdata(val_vd);
        cst_munmap_file(vd);
    }
    
    return 0;
}

