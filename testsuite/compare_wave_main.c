/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2009                             */
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
/*               Date:  November 2009                                    */
/*************************************************************************/
/*                                                                       */
/*  Compare two waveforms                                                */
/*************************************************************************/
#include <stdio.h>
#include "cst_wave.h"
#include "cst_tokenstream.h"
#include "cst_args.h"
#include "cst_math.h"

int main(int argc, char **argv)
{
    cst_wave *w1, *w2;
    cst_val *files;
    cst_features *args;
    int i1, i2, l;
    double d,s;
    double b;
    int best,x, y;
    const char *f1, *f2;
    float v1,v2,v3,c;
    float xsum,xsumsq,xcount;
    float ysum,ysumsq,ycount;
    float xxsum,xxsumsq,xxcount;
    float yysum,yysumsq,yycount;
    float xysum,xysumsq,xycount;

    args = new_features();
    files =
        cst_args(argv,argc,
                 "usage: compare_waves OPTIONS WAVE1 WAVE2\n"
                 "Compare two waveformfiles\n",
                 args);

    f1 = val_string(val_car(files));
    f2 = val_string(val_car(val_cdr(files)));
    w1 = new_wave();
    w2 = new_wave();

    if (cst_wave_load_riff(w1,f1) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "compare_wave: can't read file or wrong format \"%s\"\n",
                f1);
        return -1;
    }

    if (cst_wave_load_riff(w2,f2) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "compare_wave: can't read file or wrong format \"%s\"\n",
                f2);
        return -1;
    }
    
    s = 0.0;
    if (w1->num_samples > w2->num_samples)
        l = w2->num_samples;
    else
        l = w1->num_samples;
    b=-1;
    best = -80;

    for (y=-80; y < 80; y++)
    {
        for (i1=80,i2=80; i1 < l-160; i1++,i2++)
        {
            d = abs(w1->samples[i1+y]) - ((1.0+(x/100.0)) * (float)abs(w2->samples[i2]));
            s += d*d;
        }
        s /= (l-160)-80;
        /*        printf("%f %d %f %d\n",sqrt(s),x,b,best); */
        if (b < 0)
            b = s;  /* first time through loop */
        if (s < b)
        {
            b = s;
            best = y;
            /*            printf("New best %f x %d\n",b,best); */
        }
    }

    /* Find correlation */
    xcount = xsum = xsumsq = 0;
    ycount = ysum = ysumsq = 0;
    xycount = xysum = xysumsq = 0;
    xxcount = xxsum = xxsumsq = 0;
    yycount = yysum = yysumsq = 0;
    for (i1=80+best,i2=80; i1 < l-160; i1++,i2++)
    {
        xcount++; xsum += w1->samples[i1]; 
        xsumsq += w1->samples[i1]*w1->samples[i1];

        ycount++; ysum += w2->samples[i2]; 
        ysumsq += w2->samples[i2]*w2->samples[i2];

        xxcount++; xxsum += w1->samples[i1] * w1->samples[i1];
        xxsumsq += w1->samples[i1]*w1->samples[i1]*w1->samples[i1]*w1->samples[i1];

        yycount++; yysum += w2->samples[i2] * w2->samples[i2]; 
        yysumsq += w2->samples[i2]*w2->samples[i2] * w2->samples[i2]*w2->samples[i2];
        
        xycount++; xysum += w1->samples[i1] * w2->samples[i2];
        xysumsq += w1->samples[i1]*w1->samples[i1]*w2->samples[i2]*w2->samples[i2];

    }
    v1 = (xysum/xycount)-((xsum/xcount)*(ysum/ycount));
    v2 = (xxsum/xxcount)-((xsum/xcount)*(xsum/xcount));
    v3 = (yysum/yycount)-((ysum/ycount)*(ysum/ycount));
    c = v1/sqrt(v2*v3);
        
    printf("%2.3f %0.3f %d\n",sqrt(b),c,best);

    return 0;
}
