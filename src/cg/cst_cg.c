/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                     Copyright (c) 2007-2017                           */
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
/*             Authors:  Alan W Black (awb@cs.cmu.edu)                   */
/*                Date:  November 2007                                   */
/*************************************************************************/
/*                                                                       */
/*  Implementation of Clustergen, Statistical Parameter Synthesizer in   */
/*  Flite                                                                */
/*                                                                       */
/*  A statistical corpus based synthesizer.                              */
/*  See Black, A. (2006), CLUSTERGEN: A Statistical Parametric           */
/*  Synthesizer using Trajectory Modeling", Interspeech 2006 - ICSLP,    */
/*  Pittsburgh, PA.                                                      */
/*  http://www.cs.cmu.edu/~awb/papers/is2006/IS061394.PDF                */
/*                                                                       */
/*  Uses MLSA for resynthesis and MLPG for smoothing                     */
/*  mlsa and mlpg come from Festvox's VC code (which came in turn        */
/*  came from NITECH's HTS                                               */
/*                                                                       */
/*************************************************************************/

#include "cst_cg.h"
#include "cst_spamf0.h"
#include "cst_hrg.h"
#include "cst_utt_utils.h"
#include "cst_audio.h"

CST_VAL_REGISTER_TYPE(cg_db,cst_cg_db)

static cst_utterance *cg_make_hmmstates(cst_utterance *utt);
static cst_utterance *cg_make_params(cst_utterance *utt);
static cst_utterance *cg_predict_params(cst_utterance *utt);
static cst_utterance *cg_resynth(cst_utterance *utt);

void delete_cg_db(cst_cg_db *db)
{
    int i,j;

    if (db->freeable == 0)
        return;  /* its in the data segment, so not freeable */

    /* Woo Hoo!  We're gonna free this garbage with a big mallet */
    /* In spite of what the const qualifiers say ... */
    cst_free((void *)db->name);

    for (i=0; db->types && db->types[i]; i++)
        cst_free((void *)db->types[i]);
    cst_free((void *)db->types);

    for (j=0; j<db->num_f0_models; j++)
    {
        for (i=0; db->f0_trees[j] && db->f0_trees[j][i]; i++)
            delete_cart((cst_cart *)(void *)db->f0_trees[j][i]);
        cst_free((void *)db->f0_trees[j]);
    }
    cst_free((void *)db->f0_trees);

    for (j=0; j<db->num_param_models; j++)
    {
        for (i=0; db->param_trees[j] && db->param_trees[j][i]; i++)
            delete_cart((cst_cart *)(void *)db->param_trees[j][i]);
        cst_free((void *)db->param_trees[j]);
    }
    cst_free((void *)db->param_trees);

    if (db->spamf0)
    {
        delete_cart((cst_cart *)(void *)db->spamf0_accent_tree);
        delete_cart((cst_cart *)(void *)db->spamf0_phrase_tree);
        for (i=0; i< db->num_frames_spamf0_accent; i++)
            cst_free((void *)db->spamf0_accent_vectors[i]);
        cst_free((void *)db->spamf0_accent_vectors);
    }

    for (j=0; j<db->num_param_models; j++)
    {
        for (i=0; i<db->num_frames[j]; i++)
            cst_free((void *)db->model_vectors[j][i]);
        cst_free((void *)db->model_vectors[j]);
    }

    cst_free((void *)db->model_min);
    cst_free((void *)db->model_range);

    if (db->model_shape != CST_CG_MODEL_SHAPE_BASE_MINRANGE)
    {
        for (j = 0; j<db->num_param_models; j++)
        {
            for (i=0; i<db->num_channels[j]; i++)
                cst_free((void *)db->qtable[j][i]);
            cst_free((void *)db->qtable[j]);
        }
    }
    cst_free((void *)db->qtable);

    /* Moved to here so they can be used for the model_shape freeing */
    cst_free(db->num_channels);
    cst_free(db->num_frames);
    cst_free((void *)db->model_vectors);

    for (j = 0; j<db->num_dur_models; j++)
    {
        for (i=0; db->dur_stats[j] && db->dur_stats[j][i]; i++)
        {
            cst_free((void *)db->dur_stats[j][i]->phone);
            cst_free((void *)db->dur_stats[j][i]);
        }
        cst_free((void *)db->dur_stats[j]);
        delete_cart((cst_cart *)(void *)db->dur_cart[j]);
    }
    cst_free((void *)db->dur_stats);
    cst_free((void *)db->dur_cart);

    for (i=0; db->phone_states && db->phone_states[i]; i++)
    {
        for (j=0; db->phone_states[i][j]; j++)
            cst_free((void *)db->phone_states[i][j]);
        cst_free((void *)db->phone_states[i]);
    }
    cst_free((void *)db->phone_states);

    cst_free((void *)db->dynwin);

    for (i=0; i<db->ME_num; i++)
        cst_free((void *)db->me_h[i]);
    cst_free((void *)db->me_h);

    cst_free((void *)db);
}

/* */
cst_utterance *cg_synth(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));

    cg_make_hmmstates(utt);
    cg_make_params(utt);
    cg_predict_params(utt);
    if (cg_db->spamf0)
    {
	cst_spamf0(utt);
    }
    cg_resynth(utt);

    return utt;
}

static float cg_state_duration(cst_item *s, cst_cg_db *cg_db)
{
    float zdur, dur;
    const char *n;
    int i, x, dm;

    /* printf("awb_debug state_dur pre %s\n",item_feat_string(s,"name")); */
    for (dm=0,zdur=0.0; dm < cg_db->num_dur_models; dm++)
        zdur += val_float(cart_interpret(s,cg_db->dur_cart[dm]));
    zdur /= dm;  /* get average zdur prediction from all dur models */
    /* printf("awb_debug state_dur post %s zdur %f\n",
       item_feat_string(s,"name"),zdur); */
    n = item_feat_string(s,"name");

    /* Note we only use the dur stats from the first model, that is */
    /* correct, but wouldn't be if the dur tree was trained on different */
    /* data */
    for (x=i=0; cg_db->dur_stats[0][i]; i++)
    {
        if (cst_streq(cg_db->dur_stats[0][i]->phone,n))
        {
            x=i;
            break;
        }
    }
    if (!cg_db->dur_stats[0][i])  /* unknown type name */
        x = 0; /* shouldn't get here, and would be 0 already anyway */

    /* unz-score the zdur with the mean/stddev for the current phone */
    dur = (zdur*cg_db->dur_stats[0][x]->stddev)+cg_db->dur_stats[0][x]->mean;
    /*
    printf("awb_debug %s i %d zdur %f mean %f stddev %f dur %f\n",
           n,x,zdur,cg_db->dur_stats[0][x]->mean,
           cg_db->dur_stats[0][x]->stddev,dur);
    */

    return dur;
}

static cst_utterance *cg_make_hmmstates(cst_utterance *utt)
{
    /* Build HMM state structure below the segment structure */
    cst_cg_db *cg_db;
    cst_relation *hmmstate, *segstate;
    cst_item *seg, *s, *ss;
    const char *segname;
    int sp,p;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    hmmstate = utt_relation_create(utt,"HMMstate");
    segstate = utt_relation_create(utt,"segstate");

    for (seg = utt_rel_head(utt,"Segment"); seg; seg=item_next(seg))
    {
        ss = relation_append(segstate,seg);
        segname = item_feat_string(seg,"name");
        for (p=0; cg_db->phone_states[p]; p++)
            if (cst_streq(segname,cg_db->phone_states[p][0]))
                break;
        if (cg_db->phone_states[p] == NULL)
            p = 0;  /* unknown phoneme */
        for (sp=1; cg_db->phone_states[p][sp]; sp++)
        {
            s = relation_append(hmmstate,NULL);
            item_add_daughter(ss,s);
            item_set_string(s,"name",cg_db->phone_states[p][sp]);
            item_set_int(s,"statepos",sp);
        }
    }

    return utt;
}

static cst_utterance *cg_make_params(cst_utterance *utt)
{
    /* puts in the frame items */
    /* historically called "mcep" but can actually be any random vectors */
    cst_cg_db *cg_db;
    cst_relation *mcep, *mcep_link;
    cst_item *s, *mcep_parent, *mcep_frame;
    int num_frames;
    float start, end;
    float dur_stretch, tok_stretch, rdur;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    mcep = utt_relation_create(utt,"mcep");
    mcep_link = utt_relation_create(utt,"mcep_link");
    end = 0.0;
    num_frames = 0;
    dur_stretch = get_param_float(utt->features,"duration_stretch", 1.0);

    for (s = utt_rel_head(utt,"HMMstate"); s; s=item_next(s))
    {
        start = end;
        tok_stretch = ffeature_float(s,"R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_duration_stretch");
        if (tok_stretch == 0)
            tok_stretch = 1.0;
        rdur = tok_stretch*dur_stretch*cg_state_duration(s,cg_db);
        /* Guarantee duration to be alt least one frame */
        if (rdur < cg_db->frame_advance)
            end = start + cg_db->frame_advance;
        else
            end = start + rdur;
        item_set_float(s,"end",end);
        mcep_parent = relation_append(mcep_link, s);
        for ( ; (num_frames * cg_db->frame_advance) <= end; num_frames++ )
        {
            mcep_frame = relation_append(mcep,NULL);
            item_add_daughter(mcep_parent,mcep_frame);
            item_set_int(mcep_frame,"frame_number",num_frames);
            item_set(mcep_frame,"name",item_feat(mcep_parent,"name"));
        }
    }

    /* Copy duration up onto Segment relation */
    for (s = utt_rel_head(utt,"Segment"); s; s=item_next(s))
        item_set(s,"end",ffeature(s,"R:segstate.daughtern.end"));

    utt_set_feat_int(utt,"param_track_num_frames",num_frames);

    return utt;
}

#if CG_OLD
static int voiced_frame(cst_item *m)
{
    const char *ph_vc;
    const char *ph_cvox;

    ph_vc = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_vc");
    ph_cvox = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_cvox");

    if (cst_streq("-",ph_vc) &&
        cst_streq("-",ph_cvox))
        return 0; /* unvoiced */
    else
        return 1; /* voiced */
}
#endif

static int voiced_frame(cst_item *m)
{
    const char *ph_vc;
    const char *ph_name;

    ph_vc = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.ph_vc");
    ph_name = ffeature_string(m,"R:mcep_link.parent.R:segstate.parent.name");

    if (cst_streq(ph_name,"pau"))
        return 0; /* unvoiced */
    else if (cst_streq("+",ph_vc))
        return 1; /* voiced */
    else if (item_feat_float(m,"voicing") > 0.5)
        /* Even though the range is 0-10, I *do* mean 0.5 */
        return 1; /* voiced */
    else
        return 0; /* unvoiced */
}

static float catmull_rom_spline(float p,float p0,float p1,float p2,float p3)
/* http://www.mvps.org/directx/articles/ */
{
    float q;

    q = ( 0.5 * 
          ( ( 2.0 * p1 ) +
            ( p * (-p0 + p2) ) +
            ( (p*p) * (((2.0 * p0) - (5.0 * p1)) +
                       ((4.0 * p2) - p3))) +
            ( (p*p*p) * (-p0 +
                         ((3.0 * p1) - (3.0 * p2)) +
                         p3))));
     /*    (set! q (* 0.5 (+ (* 2 p1) 
           (* (+ (* -1 p0) p2) p)
            (* (+ (- (* 2 p0) (* 5 p1)) (- (* 4 p2) p3)) (* p p))
            (* (+ (* -1 p0) (- (* 3 p1) (* 3 p2)) p3) (* p p p)))))
     */
    return q;
}

static void cg_F0_interpolate_spline(cst_utterance *utt,
                                     cst_track *param_track)
{
    float start_f0, mid_f0, end_f0;
    int start_index, end_index, mid_index;
    int nsi, nei, nmi;  /* next syllable indices */
    float nmid_f0, pmid_f0;
    cst_item *syl;
    int i;
    float m;

    start_f0 = mid_f0 = end_f0 = -1.0;

    for (syl=utt_rel_head(utt,"Syllable"); syl; syl=item_next(syl))
    {
        start_index = ffeature_int(syl,"R:SylStructure.daughter1.R:segstate.daughter1.R:mcep_link.daughter1.frame_number");
        end_index = ffeature_int(syl,"R:SylStructure.daughtern.R:segstate.daughtern.R:mcep_link.daughtern.frame_number");
        mid_index = (int)((start_index + end_index)/2.0);
        if (end_index <= start_index)
            continue;
        
        start_f0 = param_track->frames[start_index][0];
        if (end_f0 > 0.0)
            start_f0 = end_f0;  /* not first time through */
        if (mid_f0 < 0.0)
            pmid_f0 = start_f0;  /* first time through */
        else
            pmid_f0 = mid_f0;
        mid_f0 =  param_track->frames[mid_index][0];
        if (item_next(syl)) /* not last syllable */
            end_f0 = (param_track->frames[end_index-1][0]+
                      param_track->frames[end_index][0])/2.0;
        else
            end_f0 = param_track->frames[end_index-1][0];
        nmid_f0=end_f0; /* in case there is no next syl */

        if (item_next(syl))
        {
            nsi = ffeature_int(syl,"n.R:SylStructure.daughter1.R:segstate.daughter1.R:mcep_link.daughter1.frame_number");
            nei = ffeature_int(syl,"n.R:SylStructure.daughtern.R:segstate.daughtern.R:mcep_link.daughtern.frame_number");
            nmi = (int)((nsi + nei)/2.0);
            nmid_f0 = param_track->frames[nmi][0];
        }
        /* start to mid syl */
        m = 1.0 / (mid_index - start_index);
        for (i=0; ((start_index+i)<mid_index); i++)
            param_track->frames[start_index+i][0] = 
                 catmull_rom_spline(i*m,pmid_f0,start_f0,mid_f0,end_f0);
        
        /* mid syl to end */
        m = 1.0 / (end_index - mid_index);
        for (i=0; ((mid_index+i)<end_index); i++)
            param_track->frames[mid_index+i][0] = 
                catmull_rom_spline(i*m,start_f0,mid_f0,end_f0,nmid_f0);
    }

    return;
}

#if 0
static void cg_smooth_F0_naive(cst_track *param_track)
{
    float l,s;
    int i,c;

    l = 0.0;
    for (i=0; i<param_track->num_frames-1; i++)
    {
        c = 0; s = 0;
        if (l > 0.0)
        {
            c++; s+=l;
        }
        if (param_track->frames[i+1][0] > 0.0)
        {
            c++; s+=param_track->frames[i+1][0];
        }
        l = param_track->frames[i][0];
        if (param_track->frames[i][0] > 0.0)
        {
            c++; s+=param_track->frames[i][0];
            param_track->frames[i][0] = s/c;
        }
    }

    return;
}
#endif

static void cg_smooth_F0(cst_utterance *utt,
                         cst_cg_db *cg_db,
                         cst_track *param_track)
{
    /* Smooth F0 and mark unvoice frames as 0.0 */
    cst_item *mcep;
    int i;
    float base_mean, base_stddev;

    /* cg_smooth_F0_naive(param_track); */
    
    cg_F0_interpolate_spline(utt,param_track);

    base_mean = get_param_float(utt->features,"int_f0_target_mean", cg_db->f0_mean);
    base_mean *= get_param_float(utt->features,"f0_shift", 1.0);
    base_stddev =
        get_param_float(utt->features,"int_f0_target_stddev", cg_db->f0_stddev);
#if 0
    FILE *ftt; int ii, awbi;
    ftt = cst_fopen("awb.f0",CST_OPEN_WRITE);
    printf("awb_debug saving F0\n");
    for (ii=0; ii<param_track->num_frames; ii++)
    {
        for (awbi=0; awbi<param_track->num_channels; awbi++)
            cst_fprintf(ftt,"%f ",param_track->frames[ii][awbi]);
        cst_fprintf(ftt,"\n");
        /* cst_fprintf(ftt,"%f %f\n",param_track->frames[ii][0],
           param_track->frames[ii][param_track->num_channels-2]); */
    }
    cst_fclose(ftt);
#endif

    for (i=0,mcep=utt_rel_head(utt,"mcep"); mcep; i++,mcep=item_next(mcep))
    {
        if (voiced_frame(mcep))
        {
            float mean = base_mean;
            float stddev = base_stddev;
            float local_f0_mean =
            ffeature_float(mcep,
                "R:mcep_link.parent.R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_f0_mean"
            );
            if (local_f0_mean != 0.0)
            {
                mean = local_f0_mean;
            }
            float local_f0_range =
            ffeature_float(mcep,
                "R:mcep_link.parent.R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_f0_range"
            );
            if (local_f0_range > 0.0)
            {
                /* feature_float returns 0 by default, shifted to allow 0 to be passed. */
                stddev = local_f0_range - 1.0;
            }
            /* scale the F0 -- which normally wont change it at all */
            param_track->frames[i][0] = 
                (((param_track->frames[i][0]-cg_db->f0_mean)/cg_db->f0_stddev) 
                 *stddev)+mean;
            /* Some safety checks */
            if (param_track->frames[i][0] < 50)
                param_track->frames[i][0] = 50;
            if (param_track->frames[i][0] > 700)
                param_track->frames[i][0] = 700;
        }
        else /* Unvoice it */
            param_track->frames[i][0] = 0.0;
    }

    return;
}

static int unpack_model_vector(cst_cg_db *cg_db,int pm,int f,float *v)
{
    /* This unpacked the potentially compressed/quantized data from the model */
    int i,j;

    if (cg_db->model_shape == CST_CG_MODEL_SHAPE_QUANTIZED_PARAMS)
    {
        for (i=0; i<cg_db->num_channels[pm]/2; i++)
        {
            v[i*2] = cg_db->qtable[pm][i*2][cg_db->model_vectors[pm][f][i]/256];
            v[(i*2)+1] =
                cg_db->qtable[pm][(i*2)+1][cg_db->model_vectors[pm][f][i]%256];
        }
#if 0
        printf("awb_debug %d\n",f);
        for (i=0; i<cg_db->num_channels[pm]; i++)
            printf("%f ",v[i]);
        printf("\n");
        for (i=0; i<cg_db->num_channels[pm]/2; i++)
            printf("%d %d ",cg_db->model_vectors[pm][f][i]/256,
                   cg_db->model_vectors[pm][f][i]%256);
        printf("\n");
#endif
        return 0;
    }
    if (cg_db->model_shape == CST_CG_MODEL_SHAPE_QUANTIZED_PARAMS_41)
    {
        j=1; /* skip F0 mean/stddev */
        for (i=0; i<25; i++,j++)        /* mcep static mean/stddev */
        {
            v[j*2] = cg_db->qtable[pm][j*2][cg_db->model_vectors[pm][f][i]/256];
            v[(j*2)+1] =
                cg_db->qtable[pm][(j*2)+1][cg_db->model_vectors[pm][f][i]%256];
        }
        for (i=25; i<25+12; i+=1,j+=2)  /* mcep deltas no mean/stddev */
        {
            v[(j*2)+1] = cg_db->qtable[pm][(j*2)+1][cg_db->model_vectors[pm][f][i]/256];
            v[(j*2)+3] =
                cg_db->qtable[pm][(j*2)+3][cg_db->model_vectors[pm][f][i]%256];
        }
        /* one delta, one me */
        v[(j*2)+1] = cg_db->qtable[pm][(j*2)+1][cg_db->model_vectors[pm][f][i]/256];
        v[(j*2)+2] = cg_db->qtable[pm][(j*2)+2][cg_db->model_vectors[pm][f][i]%256];
        i++; j+=2;
        /* one me, another me */
        v[(j*2)] = cg_db->qtable[pm][j*2][cg_db->model_vectors[pm][f][i]/256];
        v[(j*2)+2] = cg_db->qtable[pm][(j*2)+2][cg_db->model_vectors[pm][f][i]%256];
        i++; j+=2;
        /* one me, another me */
        v[(j*2)] = cg_db->qtable[pm][j*2][cg_db->model_vectors[pm][f][i]/256];
        v[(j*2)+2] = cg_db->qtable[pm][(j*2)+2][cg_db->model_vectors[pm][f][i]%256];
        i++; j+=2;
        /* one voicing and another v-stddef */
        v[(j*2)] = cg_db->qtable[pm][j*2][cg_db->model_vectors[pm][f][i]/256];
        v[(j*2)+1] = cg_db->qtable[pm][(j*2)+1][cg_db->model_vectors[pm][f][i]%256];
#if 0
        printf("awb_debug pm %d frame %d\n",pm,f);
        for (i=0; i<cg_db->num_channels[pm]; i++)
            printf("%f ",v[i]);
        printf("\n");
#endif
        return 0;
    }
    /* if (cg_db->model_shape == CST_CG_MODEL_SHAPE_BASE_MINRANGE) */
    else /* let's always do this second one in case model_shape isn't set */
    {
        for (i=0; i<cg_db->num_channels[pm]; i++)
        {
            v[i] = cg_db->model_min[i]+
                ((float)((cg_db->model_vectors[pm][f][i])/
                         65535.0)*cg_db->model_range[i]);
        }
        return 0;
    }
}

static cst_utterance *cg_predict_params(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cst_track *param_track;
    cst_track *str_track = NULL;
    cst_item *mcep;
    const cst_cart *mcep_tree, *f0_tree;
    int i,j,f,p,o,pm;
    const char *mname;
    float *unpacked_vector;
    float f0_val, f0_bit;
    float local_gain, voicing;
    int fff;
    int extra_feats = 0;

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    param_track = new_track();
    if (cg_db->do_mlpg) /* which should be the default */
        fff = 1;  /* copy details with stddevs */
    else
        fff = 2;  /* copy details without stddevs */

    extra_feats = 1;  /* voicing */
    if (cg_db->mixed_excitation)
    {
        extra_feats += 5;
        str_track = new_track();
        cst_track_resize(str_track,
                         utt_feat_int(utt,"param_track_num_frames"),
                         5);
    }
    
    cst_track_resize(param_track,
                     utt_feat_int(utt,"param_track_num_frames"),
                     (cg_db->num_channels[0]/fff)-
                       (2 * extra_feats));/* no voicing or str */
    unpacked_vector = cst_alloc(float,cg_db->num_channels[0]);
    f = 0;
    for (i=0,mcep=utt_rel_head(utt,"mcep"); mcep; i++,mcep=item_next(mcep))
    {
        mname = item_feat_string(mcep,"name");
        local_gain = ffeature_float(mcep,"R:mcep_link.parent.R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_gain");
        if (local_gain == 0.0) local_gain = 1.0;
        for (p=0; cg_db->types[p]; p++)
            if (cst_streq(mname,cg_db->types[p]))
                break;
        if (cg_db->types[p] == NULL)
            p=0; /* if there isn't a matching tree, use the first one */

        /* Predict F0 */
        for (f0_val=pm=0; pm<cg_db->num_f0_models; pm++)
        {
            f0_tree = cg_db->f0_trees[pm][p];
            f0_bit = val_float(cart_interpret(mcep,f0_tree));
            f0_val += f0_bit;
        }
        param_track->frames[i][0] = f0_val/cg_db->num_f0_models;
        if (param_track->frames[i][0] < 50.0)
            param_track->frames[i][0] = 0.0;
        /* what about stddev ? */

        /* We only have multiple models now, but the default is one model */
        /* Predict spectral coeffs */
        voicing = 0.0;
        for (pm=0; pm<cg_db->num_param_models; pm++)
        {
            mcep_tree = cg_db->param_trees[pm][p];
            /* printf("awb_debug mcep_tree name %s i\n",mname); */
            f = val_int(cart_interpret(mcep,mcep_tree));
            /* If there is one model this will be fine, if there are */
            /* multiple models this will be the nth model */
            item_set_int(mcep,"clustergen_param_frame",f);
            /* printf("awb_debug name %s i %d f %d\n",mname,i,f); */

            /* Unpack the model[pm][f] vector */
            unpack_model_vector(cg_db,pm,f,unpacked_vector);

            /* Old code used to average in param[0] with F0 too (???) */

            for (j=2; j<param_track->num_channels; j++)
            {
                if (pm == 0) param_track->frames[i][j] = 0.0;
                param_track->frames[i][j] += unpacked_vector[j*fff]/

                    (float)cg_db->num_param_models;
            }

            if (cg_db->mixed_excitation)
            {
                o = j;
                for (j=0; j<5; j++)
                {
                    if (pm == 0) str_track->frames[i][j] = 0.0;
                    str_track->frames[i][j] +=
                        unpacked_vector[(o+(2*j))*fff] /
                        (float)cg_db->num_param_models;
                }
            }

            /* last coefficient is average voicing for cluster */
            voicing /= (float)(pm+1);
            voicing += unpacked_vector[cg_db->num_channels[pm]-2] / 
                (float)(pm+1);
        }
        item_set_float(mcep,"voicing",voicing);
        /* Apply local gain to c0 */
        param_track->frames[i][2] *= local_gain;

        param_track->times[i] = i * cg_db->frame_advance;
    }

    cst_free(unpacked_vector);
    cg_smooth_F0(utt,cg_db,param_track);

    utt_set_feat(utt,"param_track",track_val(param_track));
    if (cg_db->mixed_excitation)
        utt_set_feat(utt,"str_track",track_val(str_track));

    return utt;
}

static cst_utterance *cg_resynth(cst_utterance *utt)
{
    cst_cg_db *cg_db;
    cst_wave *w;
    cst_track *param_track;
    cst_track *str_track = NULL;
    cst_track *smoothed_track;
    const cst_val *streaming_info_val;
    cst_audio_streaming_info *asi = NULL;
    int mlsa_speed_param = 0;

    streaming_info_val=get_param_val(utt->features,"streaming_info",NULL);
    if (streaming_info_val)
    {
        asi = val_audio_streaming_info(streaming_info_val);
        asi->utt = utt;
    }
    /* Values 5-15 might be reasonably to speed things up.  This number */
    /* is used to reduce the number of parameters used in the mceps      */
    /* e.g. value 10 will speed up from 21.0 faster than real time       */
    /* to 26.4 times faster than real time (for builtin rms) */
    mlsa_speed_param = get_param_int(utt->features,"mlsa_speed_param",0);

    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));
    param_track = val_track(utt_feat_val(utt,"param_track"));
    /* awb_debug */
    /* cst_track_save_est(param_track, "flite_pre_mlpg.track"); */
    if (cg_db->mixed_excitation)
        str_track = val_track(utt_feat_val(utt,"str_track"));

    if (cg_db->do_mlpg)
    {
        smoothed_track = mlpg(param_track, cg_db);
        /* cst_track_save_est(smoothed_track, "flite_post_mlpg.track"); */
        w = mlsa_resynthesis(smoothed_track,str_track,cg_db,
                             asi,mlsa_speed_param);
        delete_track(smoothed_track);
    }
    else
        w=mlsa_resynthesis(param_track,str_track,cg_db,
                           asi,mlsa_speed_param);

    if (w == NULL)
    {
        /* Synthesis Failed, probably because it was interrupted */
        utt_set_feat_int(utt,"Interrupted",1);
        w = new_wave();
    }

#if 0
    /* Apply local gain */
    for (i=0,tok=utt_rel_head(utt,"Token"); tok; i++,tok=item_next(tok))
    {
        if (item_feat_present(tok,"local_gain"))
            local_gain = item_feat_float(tokget_param_fffeature_float(tok,"R:mcep_link.parent.R:segstate.parent.R:SylStructure.parent.parent.R:Token.parent.local_gain");

    }
#endif

    utt_set_wave(utt,w);

    return utt;
}



