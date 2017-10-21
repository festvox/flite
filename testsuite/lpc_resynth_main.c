/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2010                             */
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
/*               Date:  April 2010                                       */
/*************************************************************************/
/*                                                                       */
/*  Resynthesize lpc with various techniques                             */
/*************************************************************************/
#include <stdio.h>
#include "flite.h"
#include "cst_track.h"
#include "cst_wave.h"
#include "cst_tokenstream.h"
#include "cst_args.h"
#include "cst_math.h"

void lpc2ref(const float *lpc, float *rfc, int order);
void ref2lpc(const float *rfc, float *lpc, int order);

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

int main(int argc, char **argv)
{
    cst_track *t1;
    cst_track *me_filters=NULL;
    cst_wave *w1, *w2, *res=NULL;
    cst_val *files;
    cst_features *args;
    int i, j;
    int order, o, s;
    int frame_length;
    float *lpcs, *residual;
    float m;
    const char *f1, *f2;
    const char *resfn = NULL;
    int last_peak = 0, next_peak;
    int period;
    float power;
    int rfc=0;
    int str=0;
    int fn, fo, ss;
    float xpulse, xnoise;
    float fxpulse, fxnoise;
    float x, me;
    float *hpulse=NULL, *hnoise=NULL;
    float *xpulsesig=NULL, *xnoisesig=NULL;
    int q=0;
    int position;
    int lpc_start = 0;

    args = new_features();
    files =
        cst_args(argv,argc,
                 "usage: lpc_resynth OPTIONS INTRACK OUTWAVE\n"
                 "Resynth an lpc track\n"
                 "-res <string> residual (as waveform)\n"
                 "-save_res Save the generated residual\n"
                 "-lpc_start <int> start of lpc params in lpc track {1}\n"
                 "-order <int> LPC order {16}\n"
                 "-str mixed excitation strengths\n"
                 "-me_filters <string> mixed excitation filters\n"
                 "-rfc Coefficents are reflection coefficients\n",
                 args);

    f1 = val_string(val_car(files));
    f2 = val_string(val_car(val_cdr(files)));
    t1 = new_track();

    lpc_start = flite_get_param_int(args,"-lpc_start",1);
    if (feat_present(args,"-rfc"))
        rfc = 1;
    if (feat_present(args,"-str"))
        str = 1;
    if (feat_present(args,"-me_filters"))
    {
        me_filters = new_track();
        if (cst_track_load_est(me_filters,flite_get_param_string(args,"-me_filters","me_filters.track")) != CST_OK_FORMAT)
        {
            fprintf(stderr,
                    "lpc_resynth: can't read file or wrong format \"%s\"\n",
                    f1);
            return -1;
        }
        hpulse = cst_alloc(float,me_filters->num_channels);
        hnoise = cst_alloc(float,me_filters->num_channels);
        xpulsesig = cst_alloc(float,me_filters->num_channels);
        xnoisesig = cst_alloc(float,me_filters->num_channels);
    }
        
    if (cst_track_load_est(t1,f1) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "lpc_resynth: can't read file or wrong format \"%s\"\n",
                f1);
        return -1;
    }

    w1 = new_wave();
    w1->sample_rate = 16000;
    cst_wave_resize(w1,160+(int)(t1->times[t1->num_frames-1]*w1->sample_rate),1);

    if (feat_present(args,"-res"))
    {
        res = new_wave();
        resfn = flite_get_param_string(args,"-res","res.wav");
	if (cst_wave_load_riff(res,resfn) != CST_OK_FORMAT)
	{
	    fprintf(stderr,
		    "lpc_resynth: can't read file or wrong format \"%s\"\n",
		    resfn);
	}
    }
    w2 = copy_wave(w1);

    order = flite_get_param_int(args,"-order",16);
    lpcs = cst_alloc(float,order);
    s = order;
    residual = NULL;

    for (i=1; i<t1->num_frames; i++)
    {
        frame_length = 10+(int)(w1->sample_rate*t1->times[i])-s;
        cst_free(residual);
        residual = cst_alloc(float,frame_length);
        /* LPC coefficients */
        if (rfc == 1)
            ref2lpc(&(t1->frames[i][lpc_start]),lpcs,order);
        else
        {
            for (o=0; o<order; o++)
                lpcs[o] = t1->frames[i][lpc_start+o];
        }

        /* Residual */
        /* Mixed excitation residual */
	if (str)
	{
	    for (fo=0; fo<me_filters->num_channels; fo++)
	    {
		hpulse[fo] = hnoise[fo] = 0.0;
                ss=order+2;
                for (fn=0; fn<me_filters->num_frames; fn++)
		{
                    me = me_filters->frames[fn][fo];
                    hpulse[fo] += t1->frames[i][ss+fn] * me;
		    hnoise[fo] += (1 - t1->frames[i][ss+fn]) * me;
		}
	    }
	    /* For each sample in the frame */
            for (j=0; j<frame_length; j++)
            {
                xnoise = plus_or_minus_one();
                if  (t1->frames[i][0] == 0)
                {
                    xpulse = 0.0; /* unvoiced */
                }
                else
                {   /* voiced */
                    period = w1->sample_rate/t1->frames[i][0];
                    next_peak = last_peak + period;
#if 0
                    if  (next_peak < (s + j))
                    {                           /* voiced peak in this frame */
                        xpulse = sqrt(period); /* sqrt ??? */
                        last_peak = next_peak;
                    }
#endif
#if 1
                    if (q == 3)
                    {
                        xpulse = sqrt(period)/4.0; q--;
                    }
                    else if (q == 2)
                    {
                        xpulse = sqrt(period)/2.0; q--;
                    }
                    else if (q == 1)
                    {
                        xpulse = sqrt(period)/4.0; q--;
                    }
                    else if (next_peak < (s + j))
                    {
                        xpulse = sqrt(period)/4.0;
                        q = 3;
                        last_peak = next_peak;
                    }
#endif
                    else
                        xpulse = 0;
                }
                fxpulse = fxnoise = 0.0;
                for (fo=me_filters->num_channels-1; fo>0; fo--)
                {
                    fxpulse += hpulse[fo] * xpulsesig[fo];
                    fxnoise += hnoise[fo] * xnoisesig[fo];
                    
                    xpulsesig[fo] = xpulsesig[fo-1];
                    xnoisesig[fo] = xnoisesig[fo-1];
                }
                fxpulse += hpulse[0] * xpulse;
                fxnoise += hnoise[0] * xnoise;
                xpulsesig[0] = xpulse;
                xnoisesig[0] = xnoise;
                
                x = fxpulse + fxnoise; /* excitation is pulse plus noise */

                residual[j] = x * sqrt(t1->frames[i][1]);
            }
        }
        /* predefined residual */
        else if (resfn)  
        {
            for (j=0; j<frame_length; j++)
                residual[j] = res->samples[s-order+j];
        }
        /* Spike plus white noise */
        else if  ((t1->frames[i][0] == 0) ||
             (t1->frames[i][t1->num_channels-1] == 0))
        {   /* unvoiced */
            printf("awb_debug unvoiced %d\n",i);
            m = sqrt(t1->frames[i][1]);
            for (j=0; j<frame_length; j++)
                residual[j] = m*rand_zero_to_one()*plus_or_minus_one();
            last_peak = s;  /* identify where next pulse should be */
        }
        else
        {   /* voiced */
            printf("awb_debug voiced %d\n",i);
            period = w1->sample_rate/t1->frames[i][0];
            next_peak = last_peak + period;
            memset(residual,0,sizeof(float)*frame_length);
            if (next_peak < (s + frame_length))
            {
                printf("awb_debug ping voiced %d\n",i);
                power = sqrt(t1->frames[i][1])*frame_length;
                if (next_peak-s < 2)
                    position=2;
                else
                    position=next_peak-s;
                
                residual[position-2] = power/4;
                residual[position-1] = power/2;
                residual[position] = power/4;
                last_peak = next_peak;
            }
        }

        /* LPC resynthesis */
        for (j=0; j< frame_length; j++)
        {
            w1->samples[s] = residual[j];
            w2->samples[s] = residual[j];
            for (o=0; (o < order) && ((s-order) > 0); o++)
                w1->samples[s] += lpcs[o]*w1->samples[s-o-1];
            s++;
        }
    }

    cst_wave_save_riff(w1,f2);
    if (feat_present(args,"-save_res"))
        cst_wave_save_riff(w2,"residual.wav");

    cst_free(residual);
    cst_free(hpulse);
    cst_free(hnoise);
    cst_free(xpulsesig);
    cst_free(xnoisesig);
    delete_features(args);
    delete_wave(w1);
    delete_wave(w2);
    delete_wave(res);
    delete_track(t1);
    delete_track(me_filters);

    return 0;
}
