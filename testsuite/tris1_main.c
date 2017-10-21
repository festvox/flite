/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2017                             */
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
/*               Date:  June 2017                                        */
/*************************************************************************/
/*                                                                       */
/*  Some clustergen model manipulation code                              */
/*************************************************************************/
#include <stdio.h>
#include "flite.h"
#include "cst_track.h"
#include "cst_ss.h"
#include "cst_tokenstream.h"
#include "cst_args.h"
#include "cst_math.h"

int main(int argc, char **argv)
{
    cst_track *bt, *nd, *ot;
    cst_val *files;
    cst_features *args;
    cst_ss ***sss;
    int i, j, f;
    const char *btf, *ndf, *otf;

    args = new_features();
    files =
        cst_args(argv,argc,
                 "usage: tris1 -base_track BT -new_data ND -out_track OT\n"
                 "Build a new param file from data\n"
                 "-base_track <string> original params file\n"
                 "-new_data <string> new data for params\n"
                 "-out_track <string> new params output file\n",
                 args);

    btf = flite_get_param_string(args,"-base_track","bt.track");
    ndf = flite_get_param_string(args,"-new_data","nd.track");
    otf = flite_get_param_string(args,"-out_track","new.track");

    bt = new_track();
    if (cst_track_load_est(bt,btf) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "tris1: can't read file or wrong format \"%s\"\n",
                btf);
        return -1;
    }
    nd = new_track();
    if (cst_track_load_est(nd,ndf) != CST_OK_FORMAT)
    {
        fprintf(stderr,
                "tris1: can't read file or wrong format \"%s\"\n",
                ndf);
        return -1;
    }
    ot = cst_track_copy(bt);

    sss = cst_alloc(cst_ss **,ot->num_frames);
    for (i=0; i<ot->num_frames; i++)
    {
        sss[i] = cst_alloc(cst_ss *,nd->num_channels-1);
        for (j=0; j<nd->num_channels-1; j++)
            sss[i][j] = new_ss();
    }

    for (i=0; i<nd->num_frames; i++)
    {
        f = nd->frames[i][0];
        /* For each coef in the frame */
        for (j=1; j<nd->num_channels; j++)
            ss_cummulate(sss[f][j-1],nd->frames[i][j]);
    }

    for (i=0; i<ot->num_frames; i++)
    {
        if (sss[i][0]->num_samples > 5)  /* enough to get some variance */
        {
            printf("awb_debug: %d %d new data\n",i,(int)sss[i][0]->num_samples);
            for (j=0; j<nd->num_channels-1; j++)
            {
                ot->frames[i][j*2] = ss_mean(sss[i][j]);
                ot->frames[i][(j*2)+1] = ss_stddev(sss[i][j]);
            }
        }
        else if (sss[i][0]->num_samples > 0)  /* enough to get some mean */
        {
            printf("awb_debug: %d %d new mean\n",i,(int)sss[i][0]->num_samples);
            for (j=0; j<nd->num_channels-1; j++)
            {
                ot->frames[i][j*2] = ss_mean(sss[i][j]);
            }
        }
        else
            printf("awb_debug: %d %d no new data\n",i,(int)sss[i][0]->num_samples);
        /* else no examples so don't update */
    }

    cst_track_save_est_binary(ot,otf);

    delete_track(ot);
    delete_track(nd);
    delete_track(bt);

    return 0;
}
