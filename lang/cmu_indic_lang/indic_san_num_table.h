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
/*  Number pronunciation for (Sanskrit) Indic                             */
/*************************************************************************/

#ifndef _indic_san_num_table_h_
#define _indic_san_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_san_digit[11][2] =
{
    { "०", "शून्य" },
    { "१", "एकं" },
    { "२", "द्वि" },
    { "३", "त्रि" },
    { "४", "चतुर्" },
    { "५", "पञ्च" },
    { "६", "षट्" },
    { "७", "सप्त" },
    { "८", "अष्ट" },
    { "९", "नव" },
    { NULL, NULL },
};

static const char * const indic_san_two_digit[101][4] =
{
    { "१", "०", "दश", NULL },
    { "१", "१", "एकादश", NULL },
    { "१", "२", "द्वादश", NULL },
    { "१", "३", "त्रयोदश", NULL },
    { "१", "४", "चतुर्दश", NULL },
    { "१", "५", "पञ्चदश", NULL },
    { "१", "६", "षोडश", NULL },
    { "१", "७", "सप्तदश", NULL },
    { "१", "८", "अष्टादश", NULL },
    { "१", "९", "एकोनविंशतिः", NULL },
    { "२", "०", "विंशतिः", NULL },
    { "२", "१", "एकाविंशतिः", NULL },
    { "२", "२", "द्वाविंशतिः", NULL },
    { "२", "३", "त्रयोविंशतिः", NULL },
    { "२", "४", "चतुर्विंशतिः", NULL },
    { "२", "५", "पञ्चविंशतिः", NULL },
    { "२", "६", "षड्विंशतिः", NULL },
    { "२", "७", "सप्तविंशतिः", NULL },
    { "२", "८", "अष्टाविंशतिः ", NULL },
    { "२", "९", "एकोनत्रिंशत्", NULL },
    { "३", "०", "त्रिंशत्", NULL },
    { "३", "१", "एकत्रिंशत्", NULL },
    { "३", "२", "द्वात्रिंशत्", NULL },
    { "३", "३", "त्रयस्त्रिंशत्", NULL },
    { "३", "४", "चतुस्त्रिंशत्", NULL },
    { "३", "५", "पञ्चत्रिंशत्", NULL },
    { "३", "६", "षट्त्रिंशत्", NULL },
    { "३", "७", "सप्तत्रिंशत्", NULL },
    { "३", "८", "अष्टात्रिंशत्", NULL },
    { "३", "९", "एकोनचत्वारिंशत्", NULL },
    { "४", "०", "चत्वारिंशत्", NULL },
    { "४", "१", "एकचत्वारिंशत्", NULL },
    { "४", "२", "द्विचत्वारिंशत्", NULL },
    { "४", "३", "त्रिचत्वारिंशत्", NULL },
    { "४", "४", "चतुश्चत्वारिंशत्", NULL },
    { "४", "५", "पञ्चचत्वारिंशत्", NULL },
    { "४", "६", "षट्चत्वारिंशत्", NULL },
    { "४", "७", "सप्तचत्वारिंशत्", NULL },
    { "४", "८", "अष्टचत्वारिंशत्", NULL },
    { "४", "९", "एकोनपञ्चाशत्", NULL },
    { "५", "०", "पञ्चाशत्", NULL },
    { "५", "१", "एकपञ्चाशत्", NULL },
    { "५", "२", "द्विपञ्चाशत्", NULL },
    { "५", "३", "त्रिपञ्चाशत्", NULL },
    { "५", "४", "चतुःपञ्चाशत्", NULL },
    { "५", "५", "पञ्चपञ्चाशत्", NULL },
    { "५", "६", "षट्पञ्चाशत्", NULL },
    { "५", "७", "सप्तपञ्चाशत्", NULL },
    { "५", "८", "अष्टपञ्चाशत्", NULL },
    { "५", "९", "एकोनषष्टिः", NULL },
    { "६", "०", "षष्टिः", NULL },
    { "६", "१", "एकषष्टिः", NULL },
    { "६", "२", "द्विषष्टिः", NULL },
    { "६", "३", "त्रिषष्टिः", NULL },
    { "६", "४", "चतुष्षष्टिः", NULL },
    { "६", "५", "पञ्चषष्टिः", NULL },
    { "६", "६", "षट्षष्टिः", NULL },
    { "६", "७", "सप्तषष्टिः", NULL },
    { "६", "८", "अष्टषष्टिः", NULL },
    { "६", "९", "एकोनसप्ततिः", NULL },
    { "७", "०", "सप्ततिः", NULL },
    { "७", "१", "एकसप्ततिः", NULL },
    { "७", "२", "द्विसप्ततिः", NULL },
    { "७", "३", "त्रिसप्ततिः", NULL },
    { "७", "४", "चतुस्सप्ततिः", NULL },
    { "७", "५", "पञ्चसप्ततिः", NULL },
    { "७", "६", "षट्सप्ततिः", NULL },
    { "७", "७", "सप्तसप्ततिः", NULL },
    { "७", "८", "अष्टसप्ततिः", NULL },
    { "७", "९", "एकोनाशीतिः", NULL },
    { "८", "०", "अशीतिः", NULL },
    { "८", "१", "एकाशीतिः", NULL },
    { "८", "२", "द्वशीतिः", NULL },
    { "८", "३", "त्र्यशीतिः", NULL },
    { "८", "४", "चतुरशीतिः", NULL },
    { "८", "५", "पञ्चाशीतिः", NULL },
    { "८", "६", "षडशीतिः", NULL },
    { "८", "७", "सप्ताशीतिः", NULL },
    { "८", "८", "अष्टाशीतिः", NULL },
    { "८", "९", "एकोननवतिः", NULL },
    { "९", "०", "नवतिः", NULL },
    { "९", "१", "एकनवतिः", NULL },
    { "९", "२", "द्विनवतिः", NULL },
    { "९", "३", "त्रिनवतिः", NULL },
    { "९", "४", "चतुर्नवतिः", NULL },
    { "९", "५", "पञ्चनवतिः", NULL },
    { "९", "६", "षण्णवतिः", NULL },
    { "९", "७", "सप्तनवतिः", NULL },
    { "९", "८", "अष्टनवतिः", NULL },
    { "९", "९", "एकोनशतम्", NULL },
    { NULL, NULL },
};

const static indic_num_table san_num_table = {
    "san",
    &indic_san_digit,
    &indic_san_two_digit,
    "शतम्",   /* hundred */
    "सहस्र", /* thousand */
    "लक्ष",  /* lakh */
    "कोटि", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
