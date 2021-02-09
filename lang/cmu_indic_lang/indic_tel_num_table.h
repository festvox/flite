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
/*  Number pronunciation for (Telugu) Indic                             */
/*************************************************************************/

#ifndef _indic_tel_num_table_h_
#define _indic_tel_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_tel_digit[11][2] =
{
    { "౦", "సున్న" },
    { "౧", "ఒకటి" },
    { "౨", "రెండు" },
    { "౩", "మూడు" },
    { "౪", "నాలుగు" },
    { "౫", "అయిదు" },
    { "౬", "ఆరు" },
    { "౭", "ఏడు" },
    { "౮", "ఎనిమిది" },
    { "౯", "తొమ్మిది" },
    { NULL, NULL },
};

static const char * const indic_tel_two_digit[101][4] =
{
    { "౧", "౦", "పది", NULL },
    { "౧", "౧", "పదకొండు", NULL },
    { "౧", "౨", "పన్నెండు", NULL },
    { "౧", "౩", "పదమూడు", NULL },
    { "౧", "౪", "పధ్నాలుగు", NULL },
    { "౧", "౫", "పదునయిదు", NULL },
    { "౧", "౬", "పదహారు", NULL },
    { "౧", "౭", "పదిహేడు", NULL },
    { "౧", "౮", "పధ్ధెనిమిది", NULL },
    { "౧", "౯", "పందొమ్మిది", NULL },
    { "౨", "౦", "ఇరవై", NULL },
    { "౨", "౧", "ఇరవై ఒకటి", NULL },
    { "౨", "౨", "ఇరవై రెండు", NULL },
    { "౨", "౩", "ఇరవై మూడు", NULL },
    { "౨", "౪", "ఇరవై నాలుగు", NULL },
    { "౨", "౫", "ఇరవై అయిదు", NULL },
    { "౨", "౬", "ఇరవై ఆరు", NULL },
    { "౨", "౭", "ఇరవై ఏడు", NULL },
    { "౨", "౮", "ఇరవై ఎనిమిది", NULL },
    { "౨", "౯", "ఇరవై తొమ్మిది", NULL },
    { "౩", "౦", "ముప్పై", NULL },
    { "౩", "౧", "ముప్పై ఒకటి", NULL },
    { "౩", "౨", "ముప్పై రెండు", NULL },
    { "౩", "౩", "ముప్పై మూడు", NULL },
    { "౩", "౪", "ముప్పై నాలుగు", NULL },
    { "౩", "౫", "ముప్పై ఐదు", NULL },
    { "౩", "౬", "ముప్పై ఆరు", NULL },
    { "౩", "౭", "ముప్పై ఏడు", NULL },
    { "౩", "౮", "ముప్పై ఎనిమిది", NULL },
    { "౩", "౯", "ముప్పై తొమ్మిది", NULL },
    { "౪", "౦", "నలభై", NULL },
    { "౪", "౧", "నలభై ఒకటి", NULL },
    { "౪", "౨", "నలభై రెండు", NULL },
    { "౪", "౩", "నలభై మూడు", NULL },
    { "౪", "౪", "నలభై నాలుగు", NULL },
    { "౪", "౫", "నలభై ఐదు", NULL },
    { "౪", "౬", "నలభై ఆరు", NULL },
    { "౪", "౭", "నలభై ఏడు", NULL },
    { "౪", "౮", "నలభై ఎనిమిది", NULL },
    { "౪", "౯", "నలభై తొమ్మిది", NULL },
    { "౫", "౦", "యాభై", NULL },
    { "౫", "౧", "యాభై ఒకటి", NULL },
    { "౫", "౨", "యాభై రెండు", NULL },
    { "౫", "౩", "యాభై మూడు", NULL },
    { "౫", "౪", "యాభై నాలుగు", NULL },
    { "౫", "౫", "యాభై ఐదు", NULL },
    { "౫", "౬", "యాభై ఆరు", NULL },
    { "౫", "౭", "యాభై ఏడు", NULL },
    { "౫", "౮", "యాభై ఎనిమిది", NULL },
    { "౫", "౯", "యాభై తొమ్మిది", NULL },
    { "౬", "౦", "అరవై", NULL },
    { "౬", "౧", "అరవై ఒకటి", NULL },
    { "౬", "౨", "అరవై రెండు", NULL },
    { "౬", "౩", "అరవై మూడు", NULL },
    { "౬", "౪", "అరవై నాలుగు", NULL },
    { "౬", "౫", "అరవై ఐదు", NULL },
    { "౬", "౬", "అరవై ఆరు", NULL },
    { "౬", "౭", "అరవై ఏడు", NULL },
    { "౬", "౮", "అరవై ఎనిమిది", NULL },
    { "౬", "౯", "అరవై తొమ్మిది", NULL },
    { "౭", "౦", "డెబ్బై", NULL },
    { "౭", "౧", "డెబ్బై ఒకటి", NULL },
    { "౭", "౨", "డెబ్బై రెండు", NULL },
    { "౭", "౩", "డెబ్బై మూడు", NULL },
    { "౭", "౪", "డెబ్బై నాలుగు", NULL },
    { "౭", "౫", "డెబ్బై ఐదు", NULL },
    { "౭", "౬", "డెబ్బై ఆరు", NULL },
    { "౭", "౭", "డెబ్బై ఏడు", NULL },
    { "౭", "౮", "డెబ్బై ఎనిమిది", NULL },
    { "౭", "౯", "డెబ్బై తొమ్మిది", NULL },
    { "౮", "౦", "ఎనభై", NULL },
    { "౮", "౧", "ఎనభై ఒకటి", NULL },
    { "౮", "౨", "ఎనభై రెండు", NULL },
    { "౮", "౩", "ఎనభై మూడు", NULL },
    { "౮", "౪", "ఎనభై నాలుగు", NULL },
    { "౮", "౫", "ఎనభై ఐదు", NULL },
    { "౮", "౬", "ఎనభై ఆరు", NULL },
    { "౮", "౭", "ఎనభై ఏడు", NULL },
    { "౮", "౮", "ఎనభై ఎనిమిది", NULL },
    { "౮", "౯", "ఎనభై తొమ్మిది", NULL },
    { "౯", "౦", "తొంభై", NULL },
    { "౯", "౧", "తొంభై ఒకటి", NULL },
    { "౯", "౨", "తొంభై రెండు", NULL },
    { "౯", "౩", "తొంభై మూడు", NULL },
    { "౯", "౪", "తొంభై నాలుగు", NULL },
    { "౯", "౫", "తొంభై ఐదు", NULL },
    { "౯", "౬", "తొంభై ఆరు", NULL },
    { "౯", "౭", "తొంభై ఏడు", NULL },
    { "౯", "౮", "తొంభై ఎనిమిది", NULL },
    { "౯", "౯", "తొంభై తొమ్మిది", NULL },
};

const static indic_num_table tel_num_table = {
    "tel",
    &indic_tel_digit,
    &indic_tel_two_digit,
    "వంద",   /* hundred */
    "వెయ్యి", /* thousand */
    "లక్ష",  /* lakh */
    "కోటి", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
