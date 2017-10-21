/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2013                            */
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
/*                                                                       */
/*  grapheme language support                                            */
/*                                                                       */
/*************************************************************************/
#include "flite.h"
#include "cst_val.h"
#include "cst_voice.h"
#include "cst_lexicon.h"
#include "cst_ffeatures.h"
#include "cmu_grapheme_lang.h"

static cst_val *cmu_grapheme_tokentowords(cst_item *token)
{
    /* Return list of words that expand token/name */
    cst_val *r;
    const char *name;

    name = item_name(token);

    if (item_feat_present(token,"phones"))
	return cons_val(string_val(name),NULL);

#if 0
    if (item_feat_present(token,"nsw"))
	nsw = item_feat_string(token,"nsw");

    utt = item_utt(token);
    lex = val_lexicon(feat_val(utt->features,"lexicon"));
#endif

    if (cst_strlen(name) > 0)
        r = cons_val(string_val(name),0);
    else
        r = NULL;
    
    return r;
}

int grapheme_utt_break(cst_tokenstream *ts,
                    const char *token,
                    cst_relation *tokens)
{
    /* We'll respect Latin punctuation */
    const char *postpunct = item_feat_string(relation_tail(tokens), "punc");
    /* const char *ltoken = item_name(relation_tail(tokens)); */

    if (cst_strchr(ts->whitespace,'\n') != cst_strrchr(ts->whitespace,'\n'))
        /* contains two new lines */
        return TRUE;
    else if (strchr(postpunct,':') ||
             strchr(postpunct,'?') ||
             strchr(postpunct,'!'))
        return TRUE;
    else if (strchr(postpunct,'.'))
        return TRUE;
    else
        return FALSE;
}

void cmu_grapheme_lang_init(cst_voice *v)
{
    /* Set grapheme language stuff */
    feat_set_string(v->features,"language","cmu_grapheme_lang");

    /* utterance break function */
    feat_set(v->features,"utt_break",breakfunc_val(&grapheme_utt_break));

    /* Phoneset -- need to get this from voice */
    feat_set(v->features,"phoneset",phoneset_val(&cmu_grapheme_phoneset));
    feat_set_string(v->features,"silence",cmu_grapheme_phoneset.silence);

    /* Get information from voice and add to lexicon */

    /* Text analyser -- whitespace defaults */
    feat_set_string(v->features,"text_whitespace",
                    cst_ts_default_whitespacesymbols);
    feat_set_string(v->features,"text_postpunctuation",
                    cst_ts_default_postpunctuationsymbols);
    feat_set_string(v->features,"text_prepunctuation",
                    cst_ts_default_prepunctuationsymbols);
    feat_set_string(v->features,"text_singlecharsymbols",
                    cst_ts_default_singlecharsymbols);

    /* Tokenization tokenization function */
    feat_set(v->features,"tokentowords_func",itemfunc_val(&cmu_grapheme_tokentowords));
      /* Pos tagger (gpos)/induced pos */
    /* Phrasing */
    feat_set(v->features,"phrasing_cart",cart_val(&cmu_grapheme_phrasing_cart));
    /* Intonation, Duration and F0 -- part of cg */
    feat_set_string(v->features,"no_intonation_accent_model","1");

    /* Default ffunctions (required) */
    basic_ff_register(v->ffunctions);

    return;
}
