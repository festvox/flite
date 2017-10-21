/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2001                             */
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
/*               Date:  June 2001                                        */
/*************************************************************************/
/*                                                                       */
/*  residuals etc                                                        */
/*                                                                       */
/*************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

/* To allow some normally const fields to manipulated during building */
#define const

#include "cst_args.h"
#include "cst_wave.h"
#include "cst_track.h"
#include "cst_sigpr.h"

float lpc_min;
float lpc_range;
char *residual_codec = "ulaw";
cst_track *vuv;

void inv_lpc_filterd(short *sig, float *a, int order, double *res, int size)
{
    /* Note this address bytes before sig, up to order shorts back */
    int i, j;
    double r;

    for (i = 0; i < size; i++)
    {
	r = sig[i];
	for (j = 1; j < order; j++)
	    r -= a[j] * (double)sig[i - j];
	res[i] = r;
    }
}

static int voiced_frame(cst_track *lpc,int i,cst_track *vuv)
{
    int p;

    p = lpc->times[i]/(vuv->times[1]-vuv->times[0]);
    
    if (p < 2)  /* some reasonable edge conditions */
        return 0; /* unvoiced */
    else if (p+2 >= vuv->num_frames)
        return 0; /* unvoiced */
    else if (vuv->frames[p][0] == 1)
        return 1; /* voiced */
    else if ((vuv->frames[p-2][0] == 0) &&
             (vuv->frames[p-1][0] == 0) &&
             (vuv->frames[p+1][0] == 0) &&
             (vuv->frames[p+2][0] == 0)
             )
        return 0; /* unvoiced */
    else 
        return 1;
}

cst_sts *find_sts(cst_wave *sig, cst_track *lpc)
{
    cst_sts *sts;
    int i,j;
    double *resd;
    int size,start,end;
    short *sigplus;
    unsigned char *ulaw_res, *power;
    int ipower;
    float sump;
    int packed_size;
#if 0
    unsigned char *residual_unpacked;
    int frame_size;
#endif

    sts = cst_alloc(cst_sts,lpc->num_frames);
    sigplus = cst_alloc(short,sig->num_samples+lpc->num_channels);
    memset(sigplus,0,sizeof(short)*lpc->num_channels);
    memmove(&sigplus[lpc->num_channels],
	    sig->samples,
	    sizeof(short)*sig->num_samples);
    /* EST LPC Windows are centered around the point */
    /* so offset things by a half period */
    start = (int)((float)sig->sample_rate * lpc->times[0]/2);
    for (i=0; i<lpc->num_frames; i++)
    {
	if (i+1 == lpc->num_frames)
	    end = (int)((float)sig->sample_rate * lpc->times[i]);
	else
	    end = (int)((float)sig->sample_rate *
			(lpc->times[i]+lpc->times[i+1]))/2;
	size = end - start;
	if (size == 0)
	    printf("frame size at %f is 0\n",lpc->times[i]);
	resd = cst_alloc(double,size);
	
	inv_lpc_filterd(&sigplus[start+lpc->num_channels],
			lpc->frames[i],lpc->num_channels,
			resd,
			size);
	sts[i].size = size;
	sts[i].frame = cst_alloc(unsigned short,lpc->num_channels-1);
	for (j=1; j < lpc->num_channels; j++)
	    sts[i].frame[j-1] = (unsigned short)
		(((lpc->frames[i][j]-lpc_min)/lpc_range)*65535);
        if (cst_streq(residual_codec,"ulaw"))
        {
            sts[i].residual = cst_alloc(unsigned char,size);
            for (j=0; j < size; j++)
                sts[i].residual[j] = cst_short_to_ulaw((short)resd[j]);
        }
        else if (cst_streq(residual_codec,"g721"))
        {
            ulaw_res = cst_alloc(unsigned char,size+CST_G721_LEADIN);
            for (j=0; j < size+CST_G721_LEADIN; j++)
                ulaw_res[j] = cst_short_to_ulaw((short)resd[j-CST_G721_LEADIN]);
            sts[i].residual = cst_g721_encode(&packed_size,size+CST_G721_LEADIN,ulaw_res);
            cst_free(ulaw_res);
        }
        else if (cst_streq(residual_codec,"g721vuv"))
        {
            sump = 0.0;
            for (j=0; j < size; j++)
                sump+=resd[j]*resd[j];
            ipower = (int)sqrt(sump/size);
            if (voiced_frame(lpc,i,vuv) || (ipower > 250))
            {
                ulaw_res = cst_alloc(unsigned char,size+CST_G721_LEADIN);
                for (j=0; j < size+CST_G721_LEADIN; j++)
                    ulaw_res[j] = cst_short_to_ulaw((short)resd[j-CST_G721_LEADIN]);
                sts[i].residual = cst_g721_encode(&packed_size,size+CST_G721_LEADIN,ulaw_res);
                cst_free(ulaw_res);
            } 
            else /* Unvoiced frame */
            {
                power = cst_alloc(unsigned char,6);
                power[0] = 0;
                power[1] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[2] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[3] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[4] = (unsigned char)ipower%256;
                sts[i].residual = power;
            }
        }
        else if (cst_streq(residual_codec,"vuv"))
        {
            sump = 0.0;
            for (j=0; j < size; j++)
                sump+=resd[j]*resd[j];
            ipower = (int)sqrt(sump/size);
            if (voiced_frame(lpc,i,vuv) || ipower > 250)
            {
                sts[i].residual = cst_alloc(unsigned char,size);
                for (j=0; j < size; j++)
                    sts[i].residual[j] = cst_short_to_ulaw((short)resd[j]);
            } 
            else /* Unvoiced frame */
            {
                sump = 0.0;
                for (j=0; j < size; j++)
                    sump+=resd[j]*resd[j];
                power = cst_alloc(unsigned char,6);
                power[0] = 0;
                power[1] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[2] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[3] = (unsigned char)ipower%256;
                ipower = ipower >> 8;
                power[4] = (unsigned char)ipower%256;
                sts[i].residual = power;
            }
        }
        else
        {
            fprintf(stderr,"unknown codec \'%s\"\n",residual_codec);
            exit(-1);
        }
	start = end; 
   }

    cst_free(sigplus);
    return sts;
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

cst_wave *reconstruct_wave(cst_wave *sig, cst_sts *sts, cst_track *lpc)
{
    cst_lpcres *lpcres;
    int i,j,r;
    int start;
    int num_samples;
    unsigned char *residual_unpacked;
    int frame_size;
    int p;
    float m, q;

    FILE *ofd; int s;

    for (num_samples = 0, i=0; i < lpc->num_frames; i++)
        num_samples += sts[i].size;

    lpcres = new_lpcres();
    lpcres_resize_frames(lpcres,lpc->num_frames);
    lpcres->num_channels = lpc->num_channels-1;
    start = (int)((float)sig->sample_rate * lpc->times[0]/2);
    num_samples += start;
    for (i=0; i<lpc->num_frames; i++)
    {
	lpcres->frames[i] = sts[i].frame;
        lpcres->sizes[i] = sts[i].size;
    }
    lpcres_resize_samples(lpcres,num_samples);
    lpcres->lpc_min = lpc_min;
    lpcres->lpc_range = lpc_range;
    lpcres->sample_rate = sig->sample_rate;
    for (r=start,i=0; i<lpc->num_frames; i++)
    {
        if (cst_streq(residual_codec,"g721"))
        {
            residual_unpacked = 
                cst_g721_decode(&frame_size, (sts[i].size+CST_G721_LEADIN+1)/2, sts[i].residual);
            for (j=0; j<sts[i].size; j++,r++)
                lpcres->residual[r] = residual_unpacked[j+CST_G721_LEADIN];
            cst_free(residual_unpacked);
        }
        else if (cst_streq(residual_codec,"g721vuv"))
        {
            if (sts[i].residual[0] == 0)
            {
                p = sts[i].residual[4]; p = p << 8;
                p += sts[i].residual[3]; p = p << 8;
                p += sts[i].residual[2]; p = p << 8;
                p += sts[i].residual[1];
                m = ((float)p);
                for (j=0; j<sts[i].size; j++,r++)
                {
                    q = m*2*rand_zero_to_one()*plus_or_minus_one();
                    lpcres->residual[r] = cst_short_to_ulaw((short)q);
                }
            }
            else
            {
                residual_unpacked = 
                    cst_g721_decode(&frame_size, (sts[i].size+CST_G721_LEADIN+1)/2, sts[i].residual);
                for (j=0; j<sts[i].size; j++,r++)
                    lpcres->residual[r] = residual_unpacked[j+CST_G721_LEADIN];
                cst_free(residual_unpacked);
            }
        }
        else if (cst_streq(residual_codec,"vuv"))
        {
            if (sts[i].residual[0] == 0)
            {
                p = sts[i].residual[4]; p = p << 8;
                p += sts[i].residual[3]; p = p << 8;
                p += sts[i].residual[2]; p = p << 8;
                p += sts[i].residual[1];
                m = ((float)p);
                for (j=0; j<sts[i].size; j++,r++)
                {
                    q = m*2*rand_zero_to_one()*plus_or_minus_one();
                    lpcres->residual[r] = cst_short_to_ulaw((short)q);
                }
            }
            else
            {
                for (j=0; j<sts[i].size; j++,r++)
                    lpcres->residual[r] = sts[i].residual[j];
            }
        }
        else /* ulaw */
        {
            for (j=0; j<sts[i].size; j++,r++)
                lpcres->residual[r] = sts[i].residual[j];
        }
    }

#if 1
    /* Debug dump */
    ofd = fopen("lpc_resid.lpc","w");
    for (s=0,i=0; i<lpcres->num_frames; i++)
    {
	fprintf(ofd,"%d %d %d\n",i,0,lpcres->sizes[i]);
	for (j=0; j < lpcres->num_channels; j++)
	    fprintf(ofd,"%d ",lpcres->frames[i][j]);
	fprintf(ofd,"\n");
        fprintf(ofd,"%d ",32000); s++;
	for (j=1; j < lpcres->sizes[i]; j++,s++)
	    fprintf(ofd,"%d ",cst_ulaw_to_short(lpcres->residual[s]));
	fprintf(ofd,"\n");
    }
    fclose(ofd);
    ofd = fopen("lpc_resid.res","w");
    for (i=0; i < r; i++)
	fprintf(ofd,"%d\n",cst_ulaw_to_short(lpcres->residual[i]));
    fclose(ofd);
#endif

    return lpc_resynth_fixedpoint(lpcres);
}

void compare_waves(cst_wave *a, cst_wave *b)
{
    int i;
    double r;

    if (a->num_samples != b->num_samples)
    {
	if (a->num_samples > b->num_samples)
	{
	    compare_waves(b,a);
	    return;
	}
    }
    
    for (r=0.0,i=0; i<a->num_samples; i++)
	r += ((float)a->samples[i]-(float)b->samples[i]) *
	    ((float)a->samples[i]-(float)b->samples[i]);
    r /= a->num_samples;
    printf("a/b diff %f\n",sqrt(r));
}

void save_sts(cst_sts *sts, cst_track *lpc, cst_wave *sig, const char *fn)
{
    FILE *fd;
    int i,j;

    if ((fd=fopen(fn,"w"))== NULL)
    {
	fprintf(stderr,"can't open for writing file: %s\n", fn);
	exit(-1);
    }

    fprintf(fd,"( %d %d %d %f %f)\n", lpc->num_frames, 
	    lpc->num_channels-1, sig->sample_rate,
	    lpc_min, lpc_range);
    for (i=0; i<lpc->num_frames; i++)
    {
	fprintf(fd,"( %f (",lpc->times[i]);
	for (j=1; j < lpc->num_channels; j++)
	    fprintf(fd," %d",sts[i].frame[j-1]);
	fprintf(fd," ) %d ( ", sts[i].size);
        if (cst_streq(residual_codec,"ulaw"))
        {
            for (j=0; j < sts[i].size; j++)
                fprintf(fd," %d",sts[i].residual[j]);
        }
        else if (cst_streq(residual_codec,"g721"))
        {
            for (j=0; j < (sts[i].size+CST_G721_LEADIN+1)/2; j++)
                fprintf(fd," %d",sts[i].residual[j]);
        }
        else if (cst_streq(residual_codec,"g721vuv"))
        {
            if (sts[i].residual[0] == 0)
                for (j=0; j < 5; j++)
                    fprintf(fd," %d",sts[i].residual[j]);
            else
                for (j=0; j < (sts[i].size+CST_G721_LEADIN+1)/2; j++)
                    fprintf(fd," %d",sts[i].residual[j]);
        }
        else if (cst_streq(residual_codec,"vuv"))
        {
            if (sts[i].residual[0] == 0)
                for (j=0; j < 5; j++)
                    fprintf(fd," %d",sts[i].residual[j]);
            else
                for (j=0; j < sts[i].size; j++)
                    fprintf(fd," %d",sts[i].residual[j]);
        }
	fprintf(fd," ))\n");
    }
    
    fclose(fd);
}

int main(int argc, char **argv)
{
    cst_track *lpc;
    cst_wave *sig, *sig2;
    cst_sts *sts;
    cst_features *args;

    args = new_features();
    cst_args(argv,argc,
             "usage: find_sts OPTIONS\n"
             "Find sts from lpc and waveform\n"
             "-codec <string>    ulaw (default), g721, g721vuv, vuv\n"
             "-vuv <string>      v/uv track (for some codecs)\n"
             "-lpcmin <float>    minimum LPC value\n"
             "-lpcrange <float>  range of LPC values\n"
             "-lpc <string>      LPC track file\n"
             "-wave <string>     WAVE file\n"
             "-o <string>        STS output file\n",
             args);

    lpc_min = get_param_float(args,"-lpcmin",-10.0);
    lpc_range = get_param_float(args,"-lpcrange",20.0);
    residual_codec = get_param_string(args,"-codec","ulaw");

    if (feat_present(args,"-vuv"))
    {
        /* Fixed frame track of voiced (1) unvoived (0) information */
        vuv = new_track();
        cst_track_load_est(vuv,get_param_string(args,"-vuv","file.vuv"));
    }

    lpc = new_track();
    cst_track_load_est(lpc,get_param_string(args,"-lpc","file.lpc"));
    sig = new_wave();
    if (cst_wave_load_riff(sig,get_param_string(args,"-wave","file.wav")) 
        == CST_WRONG_FORMAT)
    {
	fprintf(stderr,
		"cannot load waveform, format unrecognized, from \"%s\"\n",
                get_param_string(args,"-wave","file.wav"));
	exit(-1);
    }

    sts = find_sts(sig,lpc);

    /* See if it worked */
    sig2 = reconstruct_wave(sig,sts,lpc);

    compare_waves(sig,sig2);
    cst_wave_save_riff(sig2,"sig2.wav");

    save_sts(sts,lpc,sig,get_param_string(args,"-o","file.sts"));

    return 0;
}
