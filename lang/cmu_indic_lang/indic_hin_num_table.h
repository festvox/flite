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
/*  Number pronunciation for (Hindi) Indic                             */
/*************************************************************************/

#ifndef _indic_hin_num_table_h_
#define _indic_hin_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_hin_digit[11][2] =
{
    { "०", "शून्य" },
    { "१", "एक" },
    { "२", "दो" },
    { "३", "तीन" },
    { "४", "चार" },
    { "५", "पांच" },
    { "६", "छः" },
    { "७", "सात" },
    { "८", "आठ" },
    { "९", "नौ" },
    { NULL, NULL },
};

static const char * const indic_hin_two_digit[101][4] =
{
    { "१", "०", "दस", NULL },
    { "१", "१", "ग्यारह", NULL },
    { "१", "२", "बारह", NULL },
    { "१", "३", "तेरह", NULL },
    { "१", "४", "चौदह", NULL },
    { "१", "५", "पंद्रह", NULL },
    { "१", "६", "सोलह", NULL },
    { "१", "७", "सत्रह", NULL },
    { "१", "८", "अट्ठारह", NULL },
    { "१", "९", "उन्निस", NULL },
    { "२", "०", "बीस", NULL },
    { "२", "१", "इक्कीस", NULL },
    { "२", "२", "बाईस", NULL },
    { "२", "३", "तेईस", NULL },
    { "२", "४", "चौबिस", NULL },
    { "२", "५", "पच्चीस", NULL },
    { "२", "६", "छब्बीस", NULL },
    { "२", "७", "सत्ताईस", NULL },
    { "२", "८", "अट्ठाईस ", NULL },
    { "२", "९", "उनतीस", NULL },
    { "३", "०", "तीस", NULL },
    { "३", "१", "इकतीस", NULL },
    { "३", "२", "बत्तीस", NULL },
    { "३", "३", "तैंतीस", NULL },
    { "३", "४", "चौंतीस", NULL },
    { "३", "५", "पैंतीस", NULL },
    { "३", "६", "छ्त्तीस", NULL },
    { "३", "७", "सैंतीस", NULL },
    { "३", "८", "अड़तीस ", NULL },
    { "३", "९", "उनतालीस", NULL },
    { "४", "०", "चालीस", NULL },
    { "४", "१", "इकतालीस", NULL },
    { "४", "२", "बयालीस", NULL },
    { "४", "३", "तैंतालीस", NULL },
    { "४", "४", "चौवालीस", NULL },
    { "४", "५", "पैंतालीस", NULL },
    { "४", "६", "छियालीस", NULL },
    { "४", "७", "सैंतालीस", NULL },
    { "४", "८", "अड़तालीस", NULL },
    { "४", "९", "उन्चास", NULL },
    { "५", "०", "पचास", NULL },
    { "५", "१", "इक्यावन", NULL },
    { "५", "२", "बावन", NULL },
    { "५", "३", "तिरपन", NULL },
    { "५", "४", "चौवन", NULL },
    { "५", "५", "पचपन", NULL },
    { "५", "६", "छप्पन", NULL },
    { "५", "७", "सत्तावन", NULL },
    { "५", "८", "अट्ठावन", NULL },
    { "५", "९", "उनसठ", NULL },
    { "६", "०", "साठ", NULL },
    { "६", "१", "इकसठ", NULL },
    { "६", "२", "बासठ", NULL },
    { "६", "३", "तिरसठ", NULL },
    { "६", "४", "चौंसठ", NULL },
    { "६", "५", "पैंसठ", NULL },
    { "६", "६", "छियासठ", NULL },
    { "६", "७", "सड़सठ", NULL },
    { "६", "८", "अड़सठ", NULL },
    { "६", "९", "उनहत्तर", NULL },
    { "७", "०", "सत्तर", NULL },
    { "७", "१", "इकहत्तर", NULL },
    { "७", "२", "बहत्तर", NULL },
    { "७", "३", "तिहत्तर", NULL },
    { "७", "४", "चौहत्तर", NULL },
    { "७", "५", "पचहत्तर", NULL },
    { "७", "६", "छिहत्तर", NULL },
    { "७", "७", "सतहत्तर", NULL },
    { "७", "८", "अठहत्तर", NULL },
    { "७", "९", "उनासी", NULL },
    { "८", "०", "अस्सी", NULL },
    { "८", "१", "इक्यासी", NULL },
    { "८", "२", "बयासी", NULL },
    { "८", "३", "तिरासी", NULL },
    { "८", "४", "चौरासी", NULL },
    { "८", "५", "पचासी", NULL },
    { "८", "६", "छियासी", NULL },
    { "८", "७", "सतासी", NULL },
    { "८", "८", "अट्ठासी", NULL },
    { "८", "९", "नवासी", NULL },
    { "९", "०", "नब्बे", NULL },
    { "९", "१", "इक्यानबे", NULL },
    { "९", "२", "बानबे", NULL },
    { "९", "३", "तिरानबे", NULL },
    { "९", "४", "चौरानबे", NULL },
    { "९", "५", "पचांनबे", NULL },
    { "९", "६", "छियानबे", NULL },
    { "९", "७", "सत्तानबे", NULL },
    { "९", "८", "अट्ठानबे", NULL },
    { "९", "९", "निन्यानबे", NULL },
    { NULL, NULL },
};

const static indic_num_table hin_num_table = {
    "hin",
    &indic_hin_digit,
    &indic_hin_two_digit,
    "सौ",   /* hundred */
    "हज़ार", /* thousand */
    "लाख",  /* lakh */
    "करोड़", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
