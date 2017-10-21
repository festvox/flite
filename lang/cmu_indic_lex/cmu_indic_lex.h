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
/*  indic Lexicon public functions                                    */
/*************************************************************************/

#ifndef _cmu_indic_lex_h_
#define _cmu_indic_lex_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
  
#include "cst_lexicon.h"
  cst_lexicon *cmu_indic_lex_init(void);

  enum cmu_indic_char_type {
    IND_INDEPENDENT_VOWEL,
    IND_CONSONANT,
    IND_VOWEL,
    IND_ANUSWAAR,
    IND_VISARGA,
    IND_NUKTA,
    IND_AVAGRAHA,
    IND_HALANT,
    IND_DIGIT,
    IND_PUNC,
    IND_IGNORE,
    IND_ADDAK
  };

  struct cmu_indic_char {
    enum cmu_indic_char_type type;
    char phoneme[12];
  } indic_char ;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

