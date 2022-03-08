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
/*  Number pronunciation for (pannada) Indic                             */
/*************************************************************************/

#ifndef _indic_pan_reg_num_table_h_
#define _indic_pan_reg_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_pan_digit[11][2] =
{
    { "੦", "ਸਿਫਰ" },
    { "੧", "ਇੱਕ" },
    { "੨", "ਦੋ" },
    { "੩", "ਤਿੱਨ" },
    { "੪", "ਚਾਰ" },
    { "੫", "ਪੰਜ" },
    { "੬", "ਛੇ" },
    { "੭", "ਸੱਤ" },
    { "੮", "ਅੱਠ" },
    { "੯", "ਨੌਂ " },
    { NULL, NULL },
};

static const char * const indic_pan_two_digit[101][4] =
{
    { "੧", "੦", "ਦਸ", NULL },
    { "੧", "੧", "ਗਿਆਰਾਂ", NULL },
    { "੧", "੨", "ਬਾਰਾਂ", NULL },
    { "੧", "੩", "ਤੇਰਾਂ", NULL },
    { "੧", "੪", "ਚੌਦਾਂ", NULL },
    { "੧", "੫", "ਪੰਦਰਾਂ", NULL },
    { "੧", "੬", "ਸੋਲਾਂ", NULL },
    { "੧", "੭", "ਸਤਾਰਾਂ", NULL },
    { "੧", "੮", "ਅਠਾਰਾਂ", NULL },
    { "੧", "੯", "ਉੱਨੀ", NULL },
    { "੨", "੦", "ਵੀਹ", NULL },
    { "੨", "੧", "ਇੱਕੀ", NULL },
    { "੨", "੨", "ਬਾਈ", NULL },
    { "੨", "੩", "ਤੇਈ", NULL },
    { "੨", "੪", "ਚੌਬੀ", NULL },
    { "੨", "੫", "ਪੱਚੀ", NULL },
    { "੨", "੬", "ਛੱਬੀ", NULL },
    { "੨", "੭", "ਸਤਾਈ", NULL },
    { "੨", "੮", "ਅਠਾਈ", NULL },
    { "੨", "੯", "ਉਨੱਤੀ", NULL },
    { "੩", "੦", "ਤੀਹ", NULL },
    { "੩", "੧", "ਇਕੱਤੀ", NULL },
    { "੩", "੨", "ਬੱਤੀ", NULL },
    { "੩", "੩", "ਤੇਤੀ", NULL },
    { "੩", "੪", "ਚੌਂਤੀ", NULL },
    { "੩", "੫", "ਪੈਂਤੀ", NULL },
    { "੩", "੬", "ਛੱਤੀ", NULL },
    { "੩", "੭", "ਸੈਂਤੀ", NULL },
    { "੩", "੮", "ਅਠੱਤੀ", NULL },
    { "੩", "੯", "ਉਨਤਾਲੀ", NULL },
    { "੪", "੦", "ਚਾਲੀ", NULL },
    { "੪", "੧", "ਇਕਤਾਲੀ", NULL },
    { "੪", "੨", "ਬਿਆਲੀ", NULL },
    { "੪", "੩", "ਤਰਤਾਈ", NULL },
    { "੪", "੪", "ਚਤਾਲੀ", NULL },
    { "੪", "੫", "ਪਨਤਾਲੀ", NULL },
    { "੪", "੬", "ਛਿਆਲੀ", NULL },
    { "੪", "੭", "ਸਨਤਾਲੀ", NULL },
    { "੪", "੮", "ਅੜਤਾਲੀ", NULL },
    { "੪", "੯", "ਉਨੰਜਾ", NULL },
    { "੫", "੦", "ਪੰਜਾਹ", NULL },
    { "੫", "੧", "ਇਕਵੰਜਾ", NULL },
    { "੫", "੨", "ਬਵੰਜਾ", NULL },
    { "੫", "੩", "ਤਰਵੰਜਾ", NULL },
    { "੫", "੪", "ਚਰਵੰਜਾ", NULL },
    { "੫", "੫", "ਪਚਵੰਜਾ", NULL },
    { "੫", "੬", "ਛਪੰਜਾ", NULL },
    { "੫", "੭", "ਸਤਵੰਜਾ", NULL },
    { "੫", "੮", "ਅਠਵੰਜਾ", NULL },
    { "੫", "੯", "ਉਣਾਠ", NULL },
    { "੬", "੦", "ਸੱਠ", NULL },
    { "੬", "੧", "ਇਕਾਹਠ", NULL },
    { "੬", "੨", "ਬਾਹਠ", NULL },
    { "੬", "੩", "ਤਰੇਂਹਠ", NULL },
    { "੬", "੪", "ਚੌਂਹਠ", NULL },
    { "੬", "੫", "ਪੈਂਹਠ", NULL },
    { "੬", "੬", "ਛਿਆਹਠ", NULL },
    { "੬", "੭", "ਸਤਾਹਠ", NULL },
    { "੬", "੮", "ਅਠਾਹਠ", NULL },
    { "੬", "੯", "ਉਣੱਤਰ", NULL },
    { "੭", "੦", "ਸੱਤਰ", NULL },
    { "੭", "੧", "ਇਕਹੱਤਰ", NULL },
    { "੭", "੨", "ਬਹੱਤਰ", NULL },
    { "੭", "੩", "ਤਹੇਤਰ", NULL },
    { "੭", "੪", "ਚੌਹੱਤਰ", NULL },
    { "੭", "੫", "ਪੰਜੱਤਰ", NULL },
    { "੭", "੬", "ਛਿਅੱਤਰ", NULL },
    { "੭", "੭", "ਸਤੱਤਰ", NULL },
    { "੭", "੮", "ਅਠੱਤਰ", NULL },
    { "੭", "੯", "ਉਣਾਸੀ", NULL },
    { "੮", "੦", "ਅੱਸੀ", NULL },
    { "੮", "੧", "ਇਕਆਸੀ", NULL },
    { "੮", "੨", "ਬਿਆਸੀ", NULL },
    { "੮", "੩", "ਤਿਰਾਸੀ", NULL },
    { "੮", "੪", "ਚੌਰਾਸੀ", NULL },
    { "੮", "੫", "ਪਚਾਸੀ", NULL },
    { "੮", "੬", "ਛਿਆਸੀ", NULL },
    { "੮", "੭", "ਸਤਾਸੀ", NULL },
    { "੮", "੮", "ਅਠਾਸੀ", NULL },
    { "੮", "੯", "ਉਨੱਨਵੇਂ", NULL },
    { "੯", "੦", "ਨੱਬੇ", NULL },
    { "੯", "੧", "ਇਕੱਨਵੇ", NULL },
    { "੯", "੨", "ਬੱਨਵੇ", NULL },
    { "੯", "੩", "ਤਰੱਨਵੇ", NULL },
    { "੯", "੪", "ਚਰੱਨਵੇ", NULL },
    { "੯", "੫", "ਪਚੱਨਵੇ", NULL },
    { "੯", "੬", "ਛਿਅੱਨਵੇ", NULL },
    { "੯", "੭", "ਸਤੱਨਵੇ", NULL },
    { "੯", "੮", "ਅਠੱਨਵੇ", NULL },
    { "੯", "੯", "ਨੜਿੱਨਵੇ", NULL },
    { NULL, NULL },
};

const static indic_num_table pan_num_table = {
    "pan",
    &indic_pan_digit,
    &indic_pan_two_digit,
    "ਸੌ",   /* hundred */
    "ਹਜਾਰ", /* thousand */
    "ਲੱਖ",  /* lakh */
    "ਕਰੋੜ", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    

