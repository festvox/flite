/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2010                            */
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
/*               Date:  May 2010                                         */
/*************************************************************************/
/*                                                                       */
/*  Streaming and events                                                 */
/*    Outputs the token as it is spoken                                  */
/*    If the token doesn't give rise to speech -- it wont be outputed    */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "flite.h"

cst_voice *register_cmu_us_kal();

int audio_stream_chunk_by_word(const cst_wave *w, int start, int size, 
                               int last, cst_audio_streaming_info *asi)
{
    /* Called with new samples from start for size samples */
    /* last is true if this is the last segment. */
    /* This is really just an example that you can copy for your streaming */
    /* function */
    /* This particular example is *not* thread safe */
    int n;
    static cst_audiodev *ad = 0;
    float start_time;
    int start_sample;
    const char *ws, *prepunc, *token, *postpunc;

    /*    printf("in by word streaming\n"); */

    if (start == 0)
        ad = audio_open(w->sample_rate,w->num_channels,CST_AUDIO_LINEAR16);

    if (asi->item == NULL)
        asi->item = relation_head(utt_relation(asi->utt,"Token"));
    if (asi->item)
    {
        start_time = flite_ffeature_float(asi->item,"R:Token.daughter1.R:SylStructure.daughter1.daughter1.R:Segment.p.end");
        start_sample = (int)(start_time * (float)w->sample_rate);
        /*        printf("start_time %f start_sample %d start %d\n",
                  start_time,start_sample,start); */
        if ((start_sample >= start) &&
            (start_sample < start+size))
        {
            ws = flite_ffeature_string(asi->item,"whitespace");
            prepunc = flite_ffeature_string(asi->item,"prepunctuation");
            if (cst_streq("0",prepunc))
                prepunc = "";
            token = flite_ffeature_string(asi->item,"name");
            postpunc = flite_ffeature_string(asi->item,"punc");
            if (cst_streq("0",postpunc))
                postpunc = "";
            printf("%s%s%s%s",ws,prepunc,token,postpunc);
            fflush(stdout);
            asi->item = item_next(asi->item);
        }
        
    }
    n = audio_write(ad,&w->samples[start],size*sizeof(short));

    if (last == 1)
    {
        audio_close(ad);
        asi->item = NULL;
        ad = NULL;
    }

    /* if you want to stop return CST_AUDIO_STREAM_STOP */
    return CST_AUDIO_STREAM_CONT;
}



int main(int argc, char **argv)
{
    cst_voice *v;
    cst_audio_streaming_info *asi;

    if (argc != 2)
    {
	fprintf(stderr,"usage: TEXTFILE\n");
	return 1;
    }

    flite_init();

    v = register_cmu_us_kal();

    asi = new_audio_streaming_info();
    asi->asc = audio_stream_chunk_by_word;
    feat_set(v->features,"streaming_info",audio_streaming_info_val(asi));

    flite_file_to_speech(argv[1],v,"none"); /* streaming will play */

    return 0;
}
