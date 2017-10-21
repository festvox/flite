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
/*  Number pronunciation for (Kannada) Indic                             */
/*************************************************************************/

#ifndef _indic_kan_reg_num_table_h_
#define _indic_kan_reg_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_kan_digit[11][2] =
{
    { "೦", "ಸೊನ್ನೆ" },
    { "೧", "ಒಂದು" },
    { "೨", "ಎರಡು" },
    { "೩", "ಮೂರು" },
    { "೪", "ನಾಲಕ್ಕು" },
    { "೫", "ಐದು" },
    { "೬", "ಆರು" },
    { "೭", "ಏಳು" },
    { "೮", "ಎಂಟು" },
    { "೯", "ಒಂಬತ್ತು" },
    { NULL, NULL },
};

static const char * const indic_kan_two_digit[101][4] =
{
    { "೧", "೦", "ಹತ್ತು", NULL },
    { "೧", "೧", "ಹನ್ನೊಂದು", NULL },
    { "೧", "೨", "ಹನ್ನೆರಡು", NULL },
    { "೧", "೩", "ಹದಿಮೂರು", NULL },
    { "೧", "೪", "ಹದಿನಾಲ್ಕು", NULL },
    { "೧", "೫", "ಹದಿನೈದು", NULL },
    { "೧", "೬", "ಹದಿನಾರು", NULL },
    { "೧", "೭", "ಹದಿನೇಳು", NULL },
    { "೧", "೮", "ಹದಿನೆಂಟು", NULL },
    { "೧", "೯", "ಹತ್ತೊಂಬತ್ತು", NULL },
    { "೨", "೦", "ಇಪ್ಪತ್ತು", NULL },
    { "೨", "೧", "ಇಪ್ಪತ್ತೊಂದು", NULL },
    { "೨", "೨", "ಇಪ್ಪತ್ತೆರಡು", NULL },
    { "೨", "೩", "ಇಪ್ಪತ್ತಮೂರು", NULL },
    { "೨", "೪", "ಇಪ್ಪತ್ತನಾಲ್ಕು", NULL },
    { "೨", "೫", "ಇಪ್ಪತ್ತೈದು", NULL },
    { "೨", "೬", "ಇಪ್ಪತ್ತಾರು", NULL },
    { "೨", "೭", "ಇಪ್ಪತ್ತೇಳು", NULL },
    { "೨", "೮", "ಇಪ್ಪತ್ತೆಂಟು", NULL },
    { "೨", "೯", "ಇಪ್ಪತ್ತೊಂಬತ್ತು", NULL },
    { "೩", "೦", "ಮೂವತ್ತು", NULL },
    { "೩", "೧", "ಮೂವತ್ತೊಂದು", NULL },
    { "೩", "೨", "ಮೂವತ್ತೆರಡು", NULL },
    { "೩", "೩", "ಮೂವತ್ತಮೂರು", NULL },
    { "೩", "೪", "ಮೂವತ್ತನಾಲ್ಕು", NULL },
    { "೩", "೫", "ಮೂವತ್ತೈದು", NULL },
    { "೩", "೬", "ಮೂವತ್ತಾರು", NULL },
    { "೩", "೭", "ಮೂವತ್ತೇಳು", NULL },
    { "೩", "೮", "ಮೂವತ್ತೆಂಟು", NULL },
    { "೩", "೯", "ಮೂವತ್ತೊಂಬತ್ತು", NULL },
    { "೪", "೦", "ನಲವತ್ತು", NULL },
    { "೪", "೧", "ನಲವತ್ತೊಂದು", NULL },
    { "೪", "೨", "ನಲವತ್ತೆರಡು", NULL },
    { "೪", "೩", "ನಲವತ್ತಮೂರು", NULL },
    { "೪", "೪", "ನಲವತ್ತನಾಲ್ಕು", NULL },
    { "೪", "೫", "ನಲವತ್ತೈದು", NULL },
    { "೪", "೬", "ನಲವತ್ತಾರು", NULL },
    { "೪", "೭", "ನಲವತ್ತೇಳು", NULL },
    { "೪", "೮", "ನಲವತ್ತೆಂಟು", NULL },
    { "೪", "೯", "ನಲವತ್ತೊಂಬತ್ತು", NULL },
    { "೫", "೦", "ಐವತ್ತು", NULL },
    { "೫", "೧", "ಐವತ್ತೊಂದು", NULL },
    { "೫", "೨", "ಐವತ್ತೆರಡು", NULL },
    { "೫", "೩", "ಐವತ್ತಮೂರು", NULL },
    { "೫", "೪", "ಐವತ್ತನಾಲ್ಕು", NULL },
    { "೫", "೫", "ಐವತ್ತೈದು", NULL },
    { "೫", "೬", "ಐವತ್ತಾರು", NULL },
    { "೫", "೭", "ಐವತ್ತೇಳು", NULL },
    { "೫", "೮", "ಐವತ್ತೆಂಟು", NULL },
    { "೫", "೯", "ಐವತ್ತೊಂಬತ್ತು", NULL },
    { "೬", "೦", "ಅರವತ್ತು", NULL },
    { "೬", "೧", "ಅರವತ್ತೊಂದು", NULL },
    { "೬", "೨", "ಅರವತ್ತೆರಡು", NULL },
    { "೬", "೩", "ಅರವತ್ತಮೂರು", NULL },
    { "೬", "೪", "ಅರವತ್ತನಾಲ್ಕು", NULL },
    { "೬", "೫", "ಅರವತ್ತೈದು", NULL },
    { "೬", "೬", "ಅರವತ್ತಾರು", NULL },
    { "೬", "೭", "ಅರವತ್ತೇಳು", NULL },
    { "೬", "೮", "ಅರವತ್ತೆಂಟು", NULL },
    { "೬", "೯", "ಅರವತ್ತೊಂಬತ್ತು", NULL },
    { "೭", "೦", "ಎಪ್ಪತ್ತು", NULL },
    { "೭", "೧", "ಎಪ್ಪತ್ತೊಂದು", NULL },
    { "೭", "೨", "ಎಪ್ಪತ್ತೆರಡು", NULL },
    { "೭", "೩", "ಎಪ್ಪತ್ತಮೂರು", NULL },
    { "೭", "೪", "ಎಪ್ಪತ್ತನಾಲ್ಕು", NULL },
    { "೭", "೫", "ಎಪ್ಪತ್ತೈದು", NULL },
    { "೭", "೬", "ಎಪ್ಪತ್ತಾರು", NULL },
    { "೭", "೭", "ಎಪ್ಪತ್ತೇಳು", NULL },
    { "೭", "೮", "ಎಪ್ಪತ್ತೆಂಟು", NULL },
    { "೭", "೯", "ಎಪ್ಪತ್ತೊಂಬತ್ತು", NULL },
    { "೮", "೦", "ಎಂಬತ್ತು", NULL },
    { "೮", "೧", "ಎಂಬತ್ತೊಂದು", NULL },
    { "೮", "೨", "ಎಂಬತ್ತೆರಡು", NULL },
    { "೮", "೩", "ಎಂಬತ್ತಮೂರು", NULL },
    { "೮", "೪", "ಎಂಬತ್ತನಾಲ್ಕು", NULL },
    { "೮", "೫", "ಎಂಬತ್ತೈದು", NULL },
    { "೮", "೬", "ಎಂಬತ್ತಾರು", NULL },
    { "೮", "೭", "ಎಂಬತ್ತೇಳು", NULL },
    { "೮", "೮", "ಎಂಬತ್ತೆಂಟು", NULL },
    { "೮", "೯", "ಎಂಬತ್ತೊಂಬತ್ತು", NULL },
    { "೯", "೦", "ತೊಂಬತ್ತು", NULL },
    { "೯", "೧", "ತೊಂಬತ್ತೊಂದು", NULL },
    { "೯", "೨", "ತೊಂಬತ್ತೆರಡು", NULL },
    { "೯", "೩", "ತೊಂಬತ್ತಮೂರು", NULL },
    { "೯", "೪", "ತೊಂಬತ್ತನಾಲ್ಕು", NULL },
    { "೯", "೫", "ತೊಂಬತ್ತೈದು", NULL },
    { "೯", "೬", "ತೊಂಬತ್ತಾರು", NULL },
    { "೯", "೭", "ತೊಂಬತ್ತೇಳು", NULL },
    { "೯", "೮", "ತೊಂಬತ್ತೆಂಟು", NULL },
    { "೯", "೯", "ತೊಂಬತ್ತೊಂಬತ್ತು", NULL },
    { NULL, NULL },
};

const static indic_num_table kan_num_table = {
    "kan",
    &indic_kan_digit,
    &indic_kan_two_digit,
    "ನೂರು",   /* hundred */
    "ಸಾವಿರ", /* thousand */
    "ಲಕ್ಷ",  /* lakh */
    "ಕೋಟಿ", /* crore */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
