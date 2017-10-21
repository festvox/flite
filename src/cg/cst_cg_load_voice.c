/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                       Copyright (c) 2010-2013                         */
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
/*             Author:  Alok Parlikar (aup@cs.cmu.edu)                   */
/*               Date:  April 2010                                       */
/*************************************************************************/
/*                                                                       */
/*  Load a clustergen voice from a file                                  */
/*                                                                       */
/*************************************************************************/

#include "flite.h"
#include "cst_cg.h"
#include "cst_cg_map.h"

cst_lexicon *cg_init_lang_lex(cst_voice *vox,
                              const cst_lang *lang_table,
                              const char *language)
{
    cst_lexicon *lex = NULL;
    int i;

    /* Search Lang table for lang_init() and lex_init(); */
    for (i=0; lang_table[i].lang; i++)
    {
        if (cst_streq(language,lang_table[i].lang))
        {
            (lang_table[i].lang_init)(vox);
            lex = (lang_table[i].lex_init)();
            break;
        }
    }

    /* NULL if lang/lex not found */
    return lex;
}

cst_voice *cst_cg_load_voice(const char *filename,
                             const cst_lang *lang_table)
{
    cst_voice *vox;
    cst_lexicon *lex = NULL;
    int end_of_features;
    const char *language;
    const char *xname;
    cst_val *secondary_langs;
    const cst_val *sc;
    cst_cg_db *cg_db;
    char* fname;
    char* fval;
    cst_file vd;
    int byteswapped = 0;
    int r;

    vd = cst_fopen(filename,CST_OPEN_READ);
    if (vd == NULL)
    {
        cst_errmsg("Error load voice: can't open file %s\n",filename);
	return NULL;
    }

    r = cst_cg_read_header(vd);
    if (r == CST_CG_BYTESWAPPED_VOICE)
        byteswapped = 1;
    else if (r != 0)
    {
        cst_errmsg("Error load voice: %s does not have expected header\n",filename);
        cst_fclose(vd);
        return NULL;
    }

    vox = new_voice();

    /* Read voice features from the external file */
    /* Read until the feature is "end_of_features" */
    fname=NULL;
    end_of_features = 0;
    while (end_of_features == 0)
    {
	cst_read_voice_feature(vd,&fname, &fval,byteswapped);
        if (cst_streq(fname,"end_of_features"))
            end_of_features = 1;
        else
        {
            xname = feat_own_string(vox->features,fname);
            flite_feat_set_string(vox->features,xname, fval);
        }
        cst_free(fname);
        cst_free(fval);
    }

    /* Load up cg_db from external file */
    cg_db = cst_cg_load_db(vox,vd,byteswapped);

    if (cg_db == NULL)
    {
	cst_fclose(vd);
        return NULL;
    }

    /* Use the language feature to initialize the correct voice */
    language = flite_get_param_string(vox->features, "language", "");

    /* Some languages require initialization of more than one language */
    /* e.g. Indic languages require English initialization too */
    /* We initialize the secondary languages first so our last init is */
    /* the primary language */
    secondary_langs = 
        val_readlist_string(flite_get_param_string(vox->features,
                                                   "secondary_languages",""));
    for (sc = secondary_langs; sc; sc=val_cdr(sc))
    {
        cg_init_lang_lex(vox,lang_table,val_string(val_car(sc)));
    }
    delete_val(secondary_langs);

    /* Init primary language */
    lex = cg_init_lang_lex(vox,lang_table,language);

    if (lex == NULL)
    {   /* Language is not supported */
	/* Delete allocated memory in cg_db */
	cst_cg_free_db(vd,cg_db);
	cst_fclose(vd);
        cst_errmsg("Error load voice: lang/lex %s not supported in this binary\n",language);
	return NULL;	
    }

    /* Things that weren't filled in already. */
    vox->name = cg_db->name;
    flite_feat_set_string(vox->features,"name",cg_db->name);
    flite_feat_set_string(vox->features,"pathname",filename);
    
    flite_feat_set(vox->features,"lexicon",lexicon_val(lex));
    flite_feat_set(vox->features,"postlex_func",uttfunc_val(lex->postlex));

    /* No standard segment durations are needed as its done at the */
    /* HMM state level */
    flite_feat_set_string(vox->features,"no_segment_duration_model","1");
    flite_feat_set_string(vox->features,"no_f0_target_model","1");

    /* Waveform synthesis */
    flite_feat_set(vox->features,"wave_synth_func",uttfunc_val(&cg_synth));
    flite_feat_set(vox->features,"cg_db",cg_db_val(cg_db));
    flite_feat_set_int(vox->features,"sample_rate",cg_db->sample_rate);

    cst_fclose(vd);
    return vox;
}

void cst_cg_unload_voice(cst_voice *vox,cst_val *voice_list)
{
    delete_voice(vox);
}

