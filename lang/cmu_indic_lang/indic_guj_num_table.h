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
/*  Number pronunciation for (Gujarati) Indic                             */
/*************************************************************************/

#ifndef _indic_guj_num_table_h_
#define _indic_guj_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_guj_digit[11][2] =
{
    { "૦", "શૂન્ય" },
    { "૧", "એક" },
    { "૨", "બે" },
    { "૩", "ત્રણ" },
    { "૪", "ચાર" },
    { "૫", "પાંચ" },
    { "૬", "છ" },
    { "૭", "સાત" },
    { "૮", "આઠ" },
    { "૯", "નવ" },
    { NULL, NULL },
};

static const char * const indic_guj_two_digit[101][4] =
{
    { "૧", "૦", "દસ", NULL },
    { "૧", "૧", "અગિયાર", NULL },
    { "૧", "૨", "બાર", NULL },
    { "૧", "૩", "તેર", NULL },
    { "૧", "૪", "ચૌદ", NULL },
    { "૧", "૫", "પંદર", NULL },
    { "૧", "૬", "સોળ", NULL },
    { "૧", "૭", "સત્તર", NULL },
    { "૧", "૮", "અઢાર", NULL },
    { "૧", "૯", "ઓગણીસ", NULL },
    { "૨", "૦", "વીસ", NULL },
    { "૨", "૧", "એકવીસ", NULL },
    { "૨", "૨", "બાવીસ", NULL },
    { "૨", "૩", "ત્રેવીસ", NULL },
    { "૨", "૪", "ચોવીસ", NULL },
    { "૨", "૫", "પચ્ચીસ", NULL },
    { "૨", "૬", "છવ્વીસ", NULL },
    { "૨", "૭", "સત્તાવીસ", NULL },
    { "૨", "૮", "અઠ્ઠાવીસ", NULL },
    { "૨", "૯", "ઓગણત્રીસ", NULL },
    { "૩", "૦", "ત્રીસ", NULL },
    { "૩", "૧", "એકત્રીસ", NULL },
    { "૩", "૨", "બત્રીસ", NULL },
    { "૩", "૩", "તેત્રીસ", NULL },
    { "૩", "૪", "ચોત્રીસ", NULL },
    { "૩", "૫", "પાંત્રીસ", NULL },
    { "૩", "૬", "છત્રીસ", NULL },
    { "૩", "૭", "સાડત્રીસ", NULL },
    { "૩", "૮", "આડત્રીસ", NULL },
    { "૩", "૯", "ઓગણચાલીસ", NULL },
    { "૪", "૦", "ચાળીસ", NULL },
    { "૪", "૧", "એકતાળીસ", NULL },
    { "૪", "૨", "બેતાળીસ", NULL },
    { "૪", "૩", "તેતાળીસ", NULL },
    { "૪", "૪", "ચુંમાળીસ", NULL },
    { "૪", "૫", "પિસ્તાળીસ", NULL },
    { "૪", "૬", "છેતાળીસ", NULL },
    { "૪", "૭", "સુડતાળીસ", NULL },
    { "૪", "૮", "અડતાળીસ", NULL },
    { "૪", "૯", "ઓગણપચાસ", NULL },
    { "૫", "૦", "પચાસ", NULL },
    { "૫", "૧", "એકાવન", NULL },
    { "૫", "૨", "બાવન", NULL },
    { "૫", "૩", "ત્રેપન", NULL },
    { "૫", "૪", "ચોપન", NULL },
    { "૫", "૫", "પંચાવન", NULL },
    { "૫", "૬", "છપ્પન", NULL },
    { "૫", "૭", "સત્તાવન", NULL },
    { "૫", "૮", "અઠ્ઠાવન", NULL },
    { "૫", "૯", "ઓગણસાઠ", NULL },
    { "૬", "૦", "સાઠ", NULL },
    { "૬", "૧", "એકસઠ", NULL },
    { "૬", "૨", "બાસઠ", NULL },
    { "૬", "૩", "ત્રેસઠ", NULL },
    { "૬", "૪", "ચોસઠ", NULL },
    { "૬", "૫", "પાંસઠ", NULL },
    { "૬", "૬", "છાસઠ", NULL },
    { "૬", "૭", "સડસઠ", NULL },
    { "૬", "૮", "અડસઠ", NULL },
    { "૬", "૯", "ઓગણોતેર", NULL },
    { "૭", "૦", "સિત્તેર", NULL },
    { "૭", "૧", "એકોતેર", NULL },
    { "૭", "૨", "બોતેર", NULL },
    { "૭", "૩", "તોતેર", NULL },
    { "૭", "૪", "ચુંમોતેર", NULL },
    { "૭", "૫", "પંચોતેર", NULL },
    { "૭", "૬", "છોંતેર", NULL },
    { "૭", "૭", "સિત્યોતેર", NULL },
    { "૭", "૮", "ઇઠયોતેર", NULL },
    { "૭", "૯", "ઓગણ્યાએંશી", NULL },
    { "૮", "૦", "એંશી", NULL },
    { "૮", "૧", "એક્યાશી", NULL },
    { "૮", "૨", "બ્યાશી", NULL },
    { "૮", "૩", "ત્યાશી", NULL },
    { "૮", "૪", "ચોરાશી", NULL },
    { "૮", "૫", "પંચાશી", NULL },
    { "૮", "૬", "છ્યાશી", NULL },
    { "૮", "૭", "સત્યાશી", NULL },
    { "૮", "૮", "ઇઠયાશી", NULL },
    { "૮", "૯", "નેવ્યાશી", NULL },
    { "૯", "૦", "નેવું", NULL },
    { "૯", "૧", "એકાણું", NULL },
    { "૯", "૨", "બાણું", NULL },
    { "૯", "૩", "ત્રાણું", NULL },
    { "૯", "૪", "ચોરાણું", NULL },
    { "૯", "૫", "પંચાણું", NULL },
    { "૯", "૬", "છન્નું", NULL },
    { "૯", "૭", "સત્તાણું", NULL },
    { "૯", "૮", "અઠ્ઠાણું", NULL },
    { "૯", "૯", "નવ્વાણું", NULL },
    { NULL, NULL },
};

const static indic_num_table guj_num_table = {
    "guj",
    &indic_guj_digit,
    &indic_guj_two_digit,
    "સો",   /* hundred */
    "હજાર", /* thousand */
    "લાખ",  /* lakh */
    "કરોડ઼", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
