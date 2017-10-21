/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2015                             */
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
/*               Date:  August 2015                                      */
/*************************************************************************/
/*                                                                       */
/*  subtract window average from waveform                                */
/*************************************************************************/
#include <stdio.h>
#include "flite.h"
#include "cst_wave.h"
#include "cst_args.h"

int main(int argc, char **argv)
{
    cst_wave *in, *out;
    cst_val *files;
    cst_features *args;
    int i,j;
    int w, a, t;

    args = new_features();
    files =
        cst_args(argv,argc,
                 "usage: dcoffset_wave OPTIONS\n"
                 "Subtract window average from waveform\n"
		 "-i <string>  Input waveform\n"
		 "-o <string>  Output waveform\n"
		 "-w <int>     Window size (in samples)\n",
                 args);

    w = flite_get_param_int(args,"-w",20);
    in = new_wave();
    cst_wave_load_riff(in,flite_get_param_string(args,"-i","-"));

    out = copy_wave(in);

    for (i=0; i<=out->num_samples; i++)
    {
        for (t=a=0,j=i-w/2; j < i+w/2; j++)
        {
            if ((j > 0) && (j < out->num_samples))
            {
                t += 1;
                a+=in->samples[j];
            }
        }
        /*        printf("%d %d %d %d %d\n",i,out->samples[i],a/t,t,out->samples[i]-a/t); */
        out->samples[i] -= a/t;
    }

    cst_wave_save_riff(out,flite_get_param_string(args,"-o","-"));

    return 0;
}
