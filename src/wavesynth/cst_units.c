/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2001                            */
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
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  General unit functions (diphones or clunit)                          */
/*                                                                       */
/*************************************************************************/

#include "cst_math.h"
#include "cst_hrg.h"
#include "cst_utt_utils.h"
#include "cst_wave.h"
#include "cst_track.h"
#include "cst_units.h"
#include "cst_sigpr.h"

static int nearest_pm(cst_sts_list *sts_list,int start,int end,float u_index);

cst_utterance *join_units(cst_utterance *utt)
{
    /* Make a waveform form the units */
    const char *join_type;

    join_type = get_param_string(utt->features,"join_type", "modified_lpc");

    if (cst_streq(join_type,"none"))
	return utt;
#if 0
    else if (cst_streq(join_type,"windowed_join"))
	join_units_windowed(utt);
#endif
    else if (cst_streq(join_type,"simple_join"))
	join_units_simple(utt);
    else if (cst_streq(join_type,"modified_lpc"))
	join_units_modified_lpc(utt);
    
    return utt;
}

cst_utterance *join_units_simple(cst_utterance *utt)
{
    cst_wave *w = 0;
    cst_lpcres *lpcres;
    const char *resynth_type;
    const cst_val *streaming_info_val;

    resynth_type = get_param_string(utt->features,"resynth_type", "fixed");
    
    asis_to_pm(utt);
    concat_units(utt);

    lpcres = val_lpcres(utt_feat_val(utt,"target_lpcres"));

    streaming_info_val=get_param_val(utt->features,"streaming_info",NULL);
    if (streaming_info_val)
    {
        lpcres->asi = val_audio_streaming_info(streaming_info_val);
        lpcres->asi->utt = utt;
    }

    if (cst_streq(resynth_type, "fixed"))
	w = lpc_resynth_fixedpoint(lpcres); 
    else 
    {
	cst_errmsg("unknown resynthesis type %s\n", resynth_type);
	cst_error(); /* Should not happen */
    }

    utt_set_wave(utt,w);
    
    return utt;
}

cst_utterance *join_units_modified_lpc(cst_utterance *utt)
{
    cst_wave *w = 0;
    cst_lpcres *lpcres;
    const char *resynth_type;
    const cst_val *streaming_info_val;

    resynth_type = get_param_string(utt->features,"resynth_type", "float");

    f0_targets_to_pm(utt);
    concat_units(utt);

    lpcres = val_lpcres(utt_feat_val(utt,"target_lpcres"));

    streaming_info_val=get_param_val(utt->features,"streaming_info",NULL);
    if (streaming_info_val)
    {
        lpcres->asi = val_audio_streaming_info(streaming_info_val);
        lpcres->asi->utt = utt;
    }

    if (cst_streq(resynth_type, "float"))
	w = lpc_resynth(lpcres); 
    else if (cst_streq(resynth_type, "fixed"))
    {
	w = lpc_resynth_fixedpoint(lpcres); 
    }
    else 
    {
	cst_errmsg("unknown resynthesis type %s\n", resynth_type);
	cst_error(); /* Should not happen */
    }

    if (w == NULL)
    {
        /* Synthesis Failed, probably because it was interrupted */
        utt_set_feat_int(utt,"Interrupted",1);
        w = new_wave();
    }

    utt_set_wave(utt,w);
    
    return utt;
}

cst_utterance *asis_to_pm(cst_utterance *utt)
{
    /* Copy the PM structure from the units unchanged */
    cst_item *u;
    cst_lpcres *target_lpcres;
    int  unit_start, unit_end;
    int utt_pms, utt_size, i;
    cst_sts_list *sts_list;

    sts_list = val_sts_list(utt_feat_val(utt,"sts_list"));
    target_lpcres = new_lpcres();

    /* Pass one to find the size */
    utt_pms = utt_size = 0;
    for (u=relation_head(utt_relation(utt,"Unit"));
	 u; 
	 u=item_next(u))
    {
	unit_start = item_feat_int(u,"unit_start");
	unit_end = item_feat_int(u,"unit_end");
	utt_size += get_unit_size(sts_list,unit_start,unit_end);
	utt_pms += unit_end - unit_start;
	item_set_int(u,"target_end",utt_size);
    }
    lpcres_resize_frames(target_lpcres,utt_pms);

    /* Pass two to fill in the values */
    utt_pms = utt_size = 0;
    for (u=relation_head(utt_relation(utt,"Unit"));
	 u; 
	 u=item_next(u))
    {
	unit_start = item_feat_int(u,"unit_start");
	unit_end = item_feat_int(u,"unit_end");
	for (i=unit_start; i<unit_end; i++,utt_pms++)
	{
	    utt_size += get_frame_size(sts_list, i);
	    target_lpcres->times[utt_pms] = utt_size;
	}
    }
    utt_set_feat(utt,"target_lpcres",lpcres_val(target_lpcres));
    return utt;
}

cst_utterance *f0_targets_to_pm(cst_utterance *utt)
{
    cst_item *t;
    float pos,lpos,f0,lf0,m;
    double time;
    int pm;
    cst_sts_list *sts_list;
    cst_lpcres *target_lpcres;

    sts_list = val_sts_list(utt_feat_val(utt,"sts_list"));
    lpos = 0;
    lf0 = 120; /* hmm */
    pm = 0;
    time = 0;
    /* First pass to count how many pms will be required */
    for (t=relation_head(utt_relation(utt,"Target"));
	 t;
	 t=item_next(t), lf0 = f0, lpos = pos) /* changed by dhopkins */
    {
	pos = item_feat_float(t,"pos");
	f0 = item_feat_float(t,"f0");
	if (time == pos) continue;
	m = (f0-lf0)/(pos-lpos);
	for ( ; time < pos; pm++)
	{
	    time += 1/(lf0 + ((time-lpos)*m));
	}
    }
    target_lpcres = new_lpcres();
    lpcres_resize_frames(target_lpcres,pm);

    lpos = 0;
    lf0 = 120;
    pm = 0;
    time = 0;
    /* Second pass puts the values in */
    for (t=relation_head(utt_relation(utt,"Target"));
	 t;
	 t=item_next(t), lf0 = f0, lpos = pos) /* changed by dhopkins */
    {
	pos = item_feat_float(t,"pos");
	f0 = item_feat_float(t,"f0");
	if (time == pos) continue;
	m = (f0-lf0)/(pos-lpos);
	for ( ; time < pos; pm++)
	{
	    time += 1/(lf0 + ((time-lpos)*m));
	    target_lpcres->times[pm] = sts_list->sample_rate * time;
	}
    }
    utt_set_feat(utt,"target_lpcres",lpcres_val(target_lpcres));
    return utt;
}

cst_utterance *concat_units(cst_utterance *utt)
{
    cst_lpcres *target_lpcres;
    cst_item *u;
    int pm_i;
    int  unit_size, unit_start, unit_end;
    int rpos, nearest_u_pm;
    int target_end, target_start;
    float m, u_index;
    cst_sts_list *sts_list;
    const char *residual_type;

    sts_list = val_sts_list(utt_feat_val(utt,"sts_list"));
    if (sts_list->codec == NULL)
        residual_type = "ulaw";
    else
        residual_type = sts_list->codec;
    target_lpcres = val_lpcres(utt_feat_val(utt,"target_lpcres"));
    
    target_lpcres->lpc_min = sts_list->coeff_min;
    target_lpcres->lpc_range = sts_list->coeff_range;
    target_lpcres->num_channels = sts_list->num_channels;
    target_lpcres->sample_rate = sts_list->sample_rate;
    lpcres_resize_samples(target_lpcres,
			  target_lpcres->times[target_lpcres->num_frames-1]);
    if (utt_feat_val(utt,"delayed_decoding"))
    {
        target_lpcres->delayed_decoding = 1;
        target_lpcres->packed_residuals = 
            cst_alloc(const unsigned char *,target_lpcres->num_frames);
    }

    target_start = 0.0; rpos = 0; pm_i = 0; u_index = 0;
    for (u=relation_head(utt_relation(utt,"Unit")); u; u=item_next(u))
    {
	unit_start = item_feat_int(u,"unit_start");
	unit_end = item_feat_int(u,"unit_end");
	unit_size = get_unit_size(sts_list,unit_start,unit_end);
	target_end = item_feat_int(u,"target_end");
	
	u_index = 0;
	m = (float)unit_size/(float)(target_end-target_start);
/*	printf("unit_size %d start %d end %d tstart %d tend %d m %f\n",  
	unit_size, unit_start, unit_end, target_start, target_end, m); */
	for ( /* pm_start=pm_i */ ; 
	     (pm_i < target_lpcres->num_frames) &&
		 (target_lpcres->times[pm_i] <= target_end);
	     pm_i++)
	{
	    nearest_u_pm = nearest_pm(sts_list,unit_start,unit_end,u_index);
	    /* Get LPC coefs (pointer) */
	    target_lpcres->frames[pm_i] = get_sts_frame(sts_list, nearest_u_pm);
	    /* Get residual (copy) */
	    target_lpcres->sizes[pm_i] =
		target_lpcres->times[pm_i] -
		(pm_i > 0 ? target_lpcres->times[pm_i-1] : 0);
#if 0            
	    if (cst_streq(residual_type,"pulse"))
		add_residual_pulse(target_lpcres->sizes[pm_i],
				   &target_lpcres->residual[rpos],
				   get_frame_size(sts_list, nearest_u_pm),
				   get_sts_residual(sts_list, nearest_u_pm));
	    else
#endif
            if (cst_streq(residual_type,"g721"))
		add_residual_g721(target_lpcres->sizes[pm_i],
				   &target_lpcres->residual[rpos],
				   get_frame_size(sts_list, nearest_u_pm),
				   get_sts_residual(sts_list, nearest_u_pm));
	    else if (cst_streq(residual_type,"g721vuv"))
            {
                if (target_lpcres->delayed_decoding)
                {
                    target_lpcres->packed_residuals[pm_i] =
                        get_sts_residual(sts_list, nearest_u_pm);
                }
                else
                {
                    add_residual_g721vuv(target_lpcres->sizes[pm_i],
				   &target_lpcres->residual[rpos],
				   get_frame_size(sts_list, nearest_u_pm),
				   get_sts_residual(sts_list, nearest_u_pm));
                }
            }
	    else if (cst_streq(residual_type,"vuv"))
		add_residual_vuv(target_lpcres->sizes[pm_i],
                                 &target_lpcres->residual[rpos],
                                 get_frame_size(sts_list, nearest_u_pm),
                                 get_sts_residual(sts_list, nearest_u_pm));
	    /* But this requires particular layout of residuals which
	       probably isn't true */
	    /*
	    if (cst_streq(residual_type,"windowed"))
		add_residual_windowed(target_lpcres->sizes[pm_i],
				     &target_lpcres->residual[rpos],
				     get_frame_size(sts_list, nearest_u_pm),
				     get_sts_residual(sts_list, nearest_u_pm));
	    */
	    else /* default is "ulaw" */
		add_residual(target_lpcres->sizes[pm_i],
			     &target_lpcres->residual[rpos],
			     get_frame_size(sts_list, nearest_u_pm),
			     get_sts_residual(sts_list, nearest_u_pm));
	    rpos+=target_lpcres->sizes[pm_i];
	    u_index += (float)target_lpcres->sizes[pm_i]*m;
	}
	target_start = target_end;
    }
    target_lpcres->num_frames = pm_i;
    return utt;
}

static int nearest_pm(cst_sts_list *sts_list, int start,int end,float u_index)
{
    /* First the pm in unit_entry that is closest to u_index */
    int i, i_size, n_size;
    i_size = 0;

    for (i=start; i < end; i++)
    {
	n_size = i_size + get_frame_size(sts_list, i);
	if (fabs((double)(u_index-(float)i_size)) <
	    fabs((double)(u_index-(float)n_size)))
	    return i;
	i_size = n_size;
    }

    return end-1;
}

#if 0
void add_residual_windowed(int targ_size, 
			   unsigned char *targ_residual,
			   int unit_size, 
			   const unsigned char *unit_residual)
{
    /* Note this doesn't work unless the unit_residuals and consecutive */
#define DI_PI 3.14159265358979323846
    float *window, *unit, *residual;
    int i,j,k, offset, win_size;

    win_size = (targ_size*2)+1;
    window = cst_alloc(float,win_size);
    window[targ_size+1] = 1.0;
    k = DI_PI / (win_size - 1);
    for (i=0,j=win_size-1; i < targ_size+1; i++,j--)
	window[j] = window[i] = 0.54 - (0.46 * cos(k * i));

    residual = cst_alloc(float,win_size);
    for (i=0; i<win_size; i++)
	residual[i] = cst_ulaw_to_short(targ_residual[i]);

    unit = cst_alloc(float,(unit_size*2)+1);
    for (i=0; i<(unit_size*2)+1; i++)
	unit[i] = cst_ulaw_to_short(unit_residual[i]);

    if (targ_size < unit_size)
	for (i=0; i < win_size; i++)
	    residual[i] += window[i] * unit[i+(unit_size-targ_size)/2];
    else
    {
	offset = (targ_size-unit_size)/2;
	for (i=offset; i < win_size-offset; i++)
	    residual[i] += window[i] * unit[i-offset];
    }

    for (i=0; i < win_size; i++)
	targ_residual[i] = cst_short_to_ulaw((short)residual[i]);

    cst_free(window);
    cst_free(residual);
    cst_free(unit);

}
#endif

void add_residual(int targ_size, unsigned char *targ_residual,
		  int unit_size, const unsigned char *unit_residual)
{
/*    float pow_factor;
      int i; */

    if (unit_size < targ_size)
	memmove(&targ_residual[((targ_size-unit_size)/2)],
		&unit_residual[0],
		unit_size*sizeof(unsigned char));
    else
    {
	memmove(&targ_residual[0],
		&unit_residual[((unit_size-targ_size)/2)],
		targ_size*sizeof(unsigned char));
    }
#if 0
    if (unit_size < targ_size)
	memmove(&targ_residual[0],
		&unit_residual[0],
		unit_size*sizeof(unsigned char));
    else
    {
	memmove(&targ_residual[0],
		&unit_residual[0],
		targ_size*sizeof(unsigned char));
    }
#endif
}

void add_residual_g721(int targ_size, unsigned char *targ_residual,
                       int uunit_size, const unsigned char *unit_residual)
{
    /* Residual is encoded with g721 */
    unsigned char *unit_residual_unpacked;
    int unit_size;

    unit_residual_unpacked = 
        cst_g721_decode(&unit_size, (uunit_size+CST_G721_LEADIN+1)/2, unit_residual);

    if (uunit_size < targ_size)
	memmove(&targ_residual[((targ_size-uunit_size)/2)],
		&unit_residual_unpacked[CST_G721_LEADIN],
		uunit_size*sizeof(unsigned char));
    else
    {
	memmove(&targ_residual[0],
		&unit_residual_unpacked[CST_G721_LEADIN+((uunit_size-targ_size)/2)],
		targ_size*sizeof(unsigned char));
    }

    cst_free(unit_residual_unpacked);
}

static double plus_or_minus_one()
{
    /* Randomly return 1 or -1 */
    /* not sure rand() is portable */
    if (rand() > RAND_MAX/2.0)
        return 1.0;
    else
        return -1.0;
}

static double rand_zero_to_one()
{
    /* Return number between 0.0 and 1.0 */
    return rand()/(float)RAND_MAX;
}

void add_residual_g721vuv(int targ_size, unsigned char *targ_residual,
                       int uunit_size, const unsigned char *unit_residual)
{
    /* Residual is encoded with g721 */
    unsigned char *unit_residual_unpacked;
    int p, j;
    float m, q;
    int unit_size;
    int offset;

    if (unit_residual[0] == 0)
    {
        unit_size = uunit_size;
        unit_residual_unpacked = cst_alloc(unsigned char,unit_size);
        p = unit_residual[4]; p = p << 8;
        p += unit_residual[3]; p = p << 8;
        p += unit_residual[2]; p = p << 8;
        p += unit_residual[1]; 
        m = ((float)p);
        for (j=0; j<unit_size; j++)
        {
            q = m*2*rand_zero_to_one()*plus_or_minus_one();
            unit_residual_unpacked[j] = cst_short_to_ulaw((short)q);
        }
        offset = 0;
    }
    else
    {
        unit_residual_unpacked = 
            cst_g721_decode(&unit_size, (uunit_size+CST_G721_LEADIN+1)/2, unit_residual);
        offset = CST_G721_LEADIN;
    }
     
    if (uunit_size < targ_size)
	memmove(&targ_residual[((targ_size-uunit_size)/2)],
		&unit_residual_unpacked[offset],
		uunit_size*sizeof(unsigned char));
    else
    {
	memmove(&targ_residual[0],
		&unit_residual_unpacked[offset+((uunit_size-targ_size)/2)],
		targ_size*sizeof(unsigned char));
    }

    cst_free(unit_residual_unpacked);
}

void add_residual_vuv(int targ_size, unsigned char *targ_residual,
                      int uunit_size, const unsigned char *unit_residual)
{
    /* Residual is encoded with vuv */
    unsigned char *unit_residual_unpacked;
    int p, j;
    float m, q;
    int unit_size;

    if (unit_residual[0] == 0)
    {
        unit_size = uunit_size;
        unit_residual_unpacked = cst_alloc(unsigned char,unit_size);
        p = unit_residual[4]; p = p << 8;
        p += unit_residual[3]; p = p << 8;
        p += unit_residual[2]; p = p << 8;
        p += unit_residual[1]; 
        m = ((float)p);
        for (j=0; j<unit_size; j++)
        {
            q = m*2*rand_zero_to_one()*plus_or_minus_one();
            unit_residual_unpacked[j] = cst_short_to_ulaw((short)q);
        }
    }
    else
    {
        /* Put in to the unpacked -- with no unpacking */
        /* The cast is because unit_residual is const, and can't be deleted */
        unit_residual_unpacked = (unsigned char *)(void *)unit_residual;
    }
     
    if (uunit_size < targ_size)
	memmove(&targ_residual[((targ_size-uunit_size)/2)],
		&unit_residual_unpacked[0],
		uunit_size*sizeof(unsigned char));
    else
    {
	memmove(&targ_residual[0],
		&unit_residual_unpacked[((uunit_size-targ_size)/2)],
		targ_size*sizeof(unsigned char));
    }

    if (unit_residual[0] == 0)
        cst_free(unit_residual_unpacked);
}

#if 0
void add_residual_pulse(int targ_size, unsigned char *targ_residual,
			int unit_size, const unsigned char *unit_residual)
{
    int i,m;
#if UINTPTR_MAX > 0xfffffffful
    long long p;
    /* Unit residual isn't a pointer its a number, the power for the 
       the sts, yes this is hackily casting the address to a number */

    /* Need voiced and unvoiced model */
    p = (long long)unit_residual; /* I know the compiler will complain about this */
#else
    long p;
    /* Unit residual isn't a pointer its a number, the power for the 
       the sts, yes this is hackily casting the address to a number */

    /* Need voiced and unvoiced model */
    p = (long)unit_residual; /* I know the compiler will complain about this */
#endif

    if (p > 7000)  /* voiced */
    {
        i = ((targ_size-unit_size)/2);
	targ_residual[i-2] = cst_short_to_ulaw((short)(p/4));
	targ_residual[i] = cst_short_to_ulaw((short)(p/2));
	targ_residual[i+2] = cst_short_to_ulaw((short)(p/4));
    }
    else /* unvoiced */
    {
        m = p / targ_size;
        for (i=0; i<targ_size; i++)
            targ_residual[i] = 
                cst_short_to_ulaw((short)(m*plus_or_minus_one()));
    }

#if 0
    if (unit_size < targ_size)
	targ_residual[((targ_size-unit_size)/2)] 
	    = cst_short_to_ulaw((short)(int)unit_residual);
    else
	targ_residual[((unit_size-targ_size)/2)] 
	    = cst_short_to_ulaw((short)(int)unit_residual);
#endif
}
#endif

