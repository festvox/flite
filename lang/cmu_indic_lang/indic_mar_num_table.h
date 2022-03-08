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
/*  Number pronunciation for (Marathi) Indic                             */
/*************************************************************************/

#ifndef _indic_mar_num_table_h_
#define _indic_mar_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_mar_digit[11][2] =
{
    { "०", "शून्य" },
    { "१", "एक" },
    { "२", "दोन" },
    { "३", "तीन" },
    { "४", "चार" },
    { "५", "पाच" },
    { "६", "सहा" },
    { "७", "सात" },
    { "८", "आठ" },
    { "९", "नऊ" },
    { NULL, NULL },
};

static const char * const indic_mar_two_digit[101][4] =
{
    { "१", "०", "दहा",  NULL },
    { "१", "१", "अकरा",  NULL },
    { "१", "२", "बारा",  NULL },
    { "१", "३", "तेरा",  NULL },
    { "१", "४", "चौदा",  NULL },
    { "१", "५", "पंधरा",  NULL },
    { "१", "६", "सोळा",  NULL },
    { "१", "७", "सतरा",  NULL },
    { "१", "८", "अठरा",  NULL },
    { "१", "९", "एकोणीस",  NULL },
    { "२", "०", "वीस",  NULL },
    { "२", "१", "एकवीस",  NULL },
    { "२", "२", "बावीस",  NULL },
    { "२", "३", "तेवीस",  NULL },
    { "२", "४", "चोवीस",  NULL },
    { "२", "५", "पंचवीस",  NULL },
    { "२", "६", "सव्वीस",  NULL },
    { "२", "७", "सत्तावीस",  NULL },
    { "२", "८", "अठ्ठावीस",  NULL },
    { "२", "९", "एकोणतीस",  NULL },
    { "३", "०", "तीस",  NULL },
    { "३", "१", "एकतीस",  NULL },
    { "३", "२", "बत्तीस",  NULL },
    { "३", "३", "तेहेतीस",  NULL },
    { "३", "४", "चौतीस",  NULL },
    { "३", "५", "पस्तीस",  NULL },
    { "३", "६", "छत्तीस",  NULL },
    { "३", "७", "सदतीस",  NULL },
    { "३", "८", "अडतीस",  NULL },
    { "३", "९", "एकोणचाळीस",  NULL },
    { "४", "०", "चाळीस",  NULL },
    { "४", "१", "एक्केचाळीस",  NULL },
    { "४", "२", "बेचाळीस",  NULL },
    { "४", "३", "त्रेचाळीस",  NULL },
    { "४", "४", "चव्वेचाळीस",  NULL },
    { "४", "५", "पंचेचाळीस",  NULL },
    { "४", "६", "सेहेचाळीस",  NULL },
    { "४", "७", "सत्तेचाळीस",  NULL },
    { "४", "८", "अठ्ठेचाळीस",  NULL },
    { "४", "९", "एकोणपन्नास",  NULL },
    { "५", "०", "पन्नास",  NULL },
    { "५", "१", "एक्कावन्न",  NULL },
    { "५", "२", "बावन्न",  NULL },
    { "५", "३", "त्रेपन्न",  NULL },
    { "५", "४", "चोपन्न",  NULL },
    { "५", "५", "पंचावन्न",  NULL },
    { "५", "६", "छप्पन्न",  NULL },
    { "५", "७", "सत्तावन्न",  NULL },
    { "५", "८", "अठ्ठावन्न",  NULL },
    { "५", "९", "एकोणसाठ",  NULL },
    { "६", "०", "साठ",  NULL },
    { "६", "१", "एकसष्ठ",  NULL },
    { "६", "२", "बासष्ठ",  NULL },
    { "६", "३", "त्रेसष्ठ",  NULL },
    { "६", "४", "चौसष्ठ",  NULL },
    { "६", "५", "पासष्ठ",  NULL },
    { "६", "६", "सहासष्ठ",  NULL },
    { "६", "७", "सदुसष्ठ",  NULL },
    { "६", "८", "अडुसष्ठ",  NULL },
    { "६", "९", "एकोणसत्तर",  NULL },
    { "७", "०", "सत्तर",  NULL },
    { "७", "१", "एक्काहत्तर",  NULL },
    { "७", "२", "बाहत्तर",  NULL },
    { "७", "३", "त्र्याहत्तर",  NULL },
    { "७", "४", "चौर्‍याहत्तर",  NULL },
    { "७", "५", "पंच्याहत्तर",  NULL },
    { "७", "६", "शहात्तर",  NULL },
    { "७", "७", "सत्याहत्तर",  NULL },
    { "७", "८", "अठ्ठ्याहत्तर",  NULL },
    { "७", "९", "एकोणऐंशी",  NULL },
    { "८", "०", "ऐंशी",  NULL },
    { "८", "१", "एक्क्याऐंशी",  NULL },
    { "८", "२", "ब्याऐंशी",  NULL },
    { "८", "३", "त्र्याऐंशी",  NULL },
    { "८", "४", "चौऱ्याऐंशी",  NULL },
    { "८", "५", "पंच्याऐंशी",  NULL },
    { "८", "६", "शहाऐंशी",  NULL },
    { "८", "७", "सत्त्याऐंशी",  NULL },
    { "८", "८", "अठ्ठ्याऐंशी",  NULL },
    { "८", "९", "एकोणनव्वद",  NULL },
    { "९", "०", "नव्वद",  NULL },
    { "९", "१", "एक्क्याण्णव",  NULL },
    { "९", "२", "ब्याण्णव",  NULL },
    { "९", "३", "त्र्याण्णव",  NULL },
    { "९", "४", "चौऱ्याण्णव",  NULL },
    { "९", "५", "पंच्याण्णव",  NULL },
    { "९", "६", "शहाण्णव",  NULL },
    { "९", "७", "सत्त्याण्णव",  NULL },
    { "९", "८", "अठ्ठ्याण्णव",  NULL },
    { "९", "९", "नव्व्याण्णव",  NULL },
    { NULL, NULL },
};

const static indic_num_table mar_num_table = {
    "mar",
    &indic_mar_digit,
    &indic_mar_two_digit,
    "शंभर",   /* hundred */
    "हजार", /* thousand */
    "लाख",  /* lakh */
    "कोटी", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
