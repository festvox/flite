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
/*  Convert between lpc and rfc                                          */
/*************************************************************************/
#include <stdio.h>
#include "cst_track.h"
#include "cst_tokenstream.h"
#include "cst_args.h"
#include "cst_math.h"

void lpc2ref(const float *lpc, float *rfc, int order);
void ref2lpc(const float *rfc, float *lpc, int order);

int main(int argc, char **argv)
{
    cst_track *t1, *t2;
    cst_val *files;
    cst_features *args;
    int i;
    int tolpc = 0;
    const char *f1, *f2;

    args = new_features();
    files =
        cst_args(argv,argc,
                 "usage: rfc OPTIONS INTRACK1 OUTTRACK2\n"
                 "Convert between LPC and RFC\n"
		 "-tolpc    from RFC to LPC\n"
		 "-torfc    from LPC to RFC\n",
                 args);

    f1 = val_string(val_car(files));
    f2 = val_string(val_car(val_cdr(files)));
    t1 = new_track();

    if (cst_track_load_est(t1,f1) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "rfc: can't read file or wrong format \"%s\"\n",
                f1);
        return -1;
    }
    if (feat_present(args,"-tolpc"))
        tolpc = 1;
    if (feat_present(args,"-torfc"))
        tolpc = 0;

    t2 = cst_track_copy(t1);

    for (i=0; i<t2->num_frames; i++)
    {
        if (tolpc)
            ref2lpc(&(t1->frames[i][1]),&(t2->frames[i][1]),t2->num_channels-1);
        else
            lpc2ref(&(t1->frames[i][1]),&(t2->frames[i][1]),t2->num_channels-1);

        
    }

    cst_track_save_est(t2,f2);

    return 0;
}
