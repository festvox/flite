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
/*  Number pronunciation for (English) Indic                             */
/*************************************************************************/

#ifndef _indic_eng_num_table_h_
#define _indic_eng_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_eng_digit[11][2] =
{
    { "0", "zero" },
    { "1", "one" },
    { "2", "two" },
    { "3", "three" },
    { "4", "four" },
    { "5", "five" },
    { "6", "six" },
    { "7", "seven" },
    { "8", "eight" },
    { "9", "nine" },
    { NULL, NULL },
};

static const char * const indic_eng_two_digit[101][4] =
{
    { "0", "0", NULL, NULL },
    { "0", "1", "and", "one" },
    { "0", "2", "and", "two" },
    { "0", "3", "and", "three" },
    { "0", "4", "and", "four" },
    { "0", "5", "and", "five" },
    { "0", "6", "and", "six" },
    { "0", "7", "and", "seven" },
    { "0", "8", "and", "eight" },
    { "0", "9", "and", "nine" },
    { "1", "0", "ten", NULL },
    { "1", "1", "eleven", NULL },
    { "1", "2", "twelve", NULL },
    { "1", "3", "thirteen", NULL },
    { "1", "4", "fourteen", NULL },
    { "1", "5", "fifteen", NULL },
    { "1", "6", "sixteen", NULL },
    { "1", "7", "seventeen", NULL },
    { "1", "8", "eighteen", NULL },
    { "1", "9", "nineteen", NULL },
    { "2", "0", "twenty", NULL },
    { "2", "1", "twenty", "one" },
    { "2", "2", "twenty", "two" },
    { "2", "3", "twenty", "three" },
    { "2", "4", "twenty", "four" },
    { "2", "5", "twenty", "five" },
    { "2", "6", "twenty", "six" },
    { "2", "7", "twenty", "seven" },
    { "2", "8", "twenty", "eight" },
    { "2", "9", "twenty", "nine" },
    { "3", "0", "thirty", NULL },
    { "3", "1", "thirty", "one" },
    { "3", "2", "thirty", "two" },
    { "3", "3", "thirty", "three" },
    { "3", "4", "thirty", "four" },
    { "3", "5", "thirty", "five" },
    { "3", "6", "thirty", "six" },
    { "3", "7", "thirty", "seven" },
    { "3", "8", "thirty", "eight" },
    { "3", "9", "thirty", "nine" },
    { "4", "0", "forty", NULL },
    { "4", "1", "forty", "one" },
    { "4", "2", "forty", "two" },
    { "4", "3", "forty", "three" },
    { "4", "4", "forty", "four" },
    { "4", "5", "forty", "five" },
    { "4", "6", "forty", "six" },
    { "4", "7", "forty", "seven" },
    { "4", "8", "forty", "eight" },
    { "4", "9", "forty", "nine" },
    { "5", "0", "fifty", NULL },
    { "5", "1", "fifty", "one" },
    { "5", "2", "fifty", "two" },
    { "5", "3", "fifty", "three" },
    { "5", "4", "fifty", "four" },
    { "5", "5", "fifty", "five" },
    { "5", "6", "fifty", "six" },
    { "5", "7", "fifty", "seven" },
    { "5", "8", "fifty", "eight" },
    { "5", "9", "fifty", "nine" },
    { "6", "0", "sixty", NULL },
    { "6", "1", "sixty", "one" },
    { "6", "2", "sixty", "two" },
    { "6", "3", "sixty", "three" },
    { "6", "4", "sixty", "four" },
    { "6", "5", "sixty", "five" },
    { "6", "6", "sixty", "six" },
    { "6", "7", "sixty", "seven" },
    { "6", "8", "sixty", "eight" },
    { "6", "9", "sixty", "nine" },
    { "7", "0", "twenty", NULL },
    { "7", "1", "seventy", "one" },
    { "7", "2", "seventy", "two" },
    { "7", "3", "seventy", "three" },
    { "7", "4", "seventy", "four" },
    { "7", "5", "seventy", "five" },
    { "7", "6", "seventy", "six" },
    { "7", "7", "seventy", "seven" },
    { "7", "8", "seventy", "eight" },
    { "7", "9", "seventy", "nine" },
    { "8", "0", "eighty", NULL },
    { "8", "1", "eighty", "one" },
    { "8", "2", "eighty", "two" },
    { "8", "3", "eighty", "three" },
    { "8", "4", "eighty", "four" },
    { "8", "5", "eighty", "five" },
    { "8", "6", "eighty", "six" },
    { "8", "7", "eighty", "seven" },
    { "8", "8", "eighty", "eight" },
    { "8", "9", "eighty", "nine" },
    { "9", "0", "ninety", NULL },
    { "9", "1", "ninety", "one" },
    { "9", "2", "ninety", "two" },
    { "9", "3", "ninety", "three" },
    { "9", "4", "ninety", "four" },
    { "9", "5", "ninety", "five" },
    { "9", "6", "ninety", "six" },
    { "9", "7", "ninety", "seven" },
    { "9", "8", "ninety", "eight" },
    { "9", "9", "ninety", "nine" },
    { NULL, NULL },
};

const static indic_num_table eng_num_table = {
    "eng",
    &indic_eng_digit,
    &indic_eng_two_digit,
    "hundred",
    "thousand",
    "lakh",
    "crore"
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif
