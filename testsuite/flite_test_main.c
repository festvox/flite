/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2015                            */
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
/*               Date:  February 2015                                    */
/*************************************************************************/
/*                                                                       */
/*  A minmal example test program                                        */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include "flite.h"

void usenglish_init(cst_voice *v);
cst_lexicon *cmulex_init(void);

int main(int argc, char **argv)
{
    cst_voice *v;
    cst_wave *w;
    cst_utterance *u;
    const char *voice_pathname;
    const char *text;
    const char *outfile;

    if (argc != 4)
    {
	fprintf(stderr,"usage: VOICE.flitevox TEXT WAVEFILE\n");
	return 1;
    }

    voice_pathname = argv[1]; /* pathname to .flitevox file */
    text = argv[2];           /* text to be synthesized */
    outfile = argv[3];        /* output file (or "play" or "none") */

    /* Initialize Flite, and set up language and lexicon */
    flite_init();
    flite_add_lang("eng",usenglish_init,cmulex_init);
    flite_add_lang("usenglish",usenglish_init,cmulex_init);

    /* Load and select voice */
    v = flite_voice_select(voice_pathname);
    if (v == NULL)
    {
        fprintf(stderr,"failed to load voice: %s\n",voice_pathname);
        return 1;
    }

    u = flite_synth_text(text,v);
    w = utt_wave(u);

    /* Play the resulting wave, save it to a filename, or do none of these */
    if (cst_streq(outfile,"play"))
        play_wave(w);
    else if (!cst_streq(outfile,"none")) 
       cst_wave_save_riff(w,outfile);

    delete_utterance(u); /* will delete w too */

    return 0;
}
