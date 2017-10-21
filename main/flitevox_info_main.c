/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2014                            */
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
/*               Date:  November 2014                                     */
/*************************************************************************/
/*                                                                       */
/*  For setting and viewing features in a .flitevox clustergen file      */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "cst_args.h"
#include "flite.h"

void *flite_set_lang_list(void);

int main(int argc, char **argv)
{
    cst_features *args=new_features();
    cst_voice *v;
    const char *voice_name, *feat;

    (void)cst_args(argv,argc,
                 "usage: flitevox_info [OPTIONS]\n"
                 "-voice <string> Pathname to flitevox file\n"
                 "-set <string>  Set given feature name\n"
                 "-get <string>  Get given feature name\n"
                 "-val <string>  Value to set\n"
                 "-info          Output general info on voice\n"
                 "set/get features in a flitevox voice.",
                 args);

    flite_init();
    flite_set_lang_list();

    if (!feat_present(args,"-voice"))
    {
	fprintf(stderr,"no voice specified\n");
	exit(-1);
    }
    voice_name = feat_string(args,"-voice");
    v = flite_voice_load(voice_name);
    if (v == NULL)
    {
	fprintf(stderr,"can't load voice %s\n",voice_name);
	exit(-1);
    }

    if (feat_present(args,"-info"))
    {
        cst_feat_print(stdout,v->features);
    } else if (feat_present(args,"-set"))
    {
        feat = get_param_string(args,"-set","feat");
        if (!feat_present(args,"-val"))
        {
            fprintf(stderr,"no feat val given for %s\n",feat);
            exit(-1);
        }
        /* set the feature */
        feat_set_string(v->features,feat,feat_string(args,"-val"));

        printf("awb_debug: setting %s %s\n",feat,
               feat_string(v->features,feat));
        /* save the voice back out again */
        flite_voice_dump(v,voice_name);
    }
    else if (feat_present(args,"-get"))
    {
        feat = get_param_string(args,"-get","feat");
        printf("%s \"%s\"\n",feat,feat_string(v->features,feat));
    }

    delete_voice(v);
    delete_features(args);

    return 0;

}
