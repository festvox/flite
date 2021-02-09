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
/*  Number pronunciation for (Tamil) Indic                             */
/*************************************************************************/

#ifndef _indic_tam_reg_num_table_h_
#define _indic_tam_reg_num_table_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "indic_num_table.h"

static const char * const indic_tam_digit[11][2] =
{
    { "௦", "பூஜ்ஜியம்" },
    { "௧", "ஒன்று" },
    { "௨", "இரண்டு" },
    { "௩", "மூன்று" },
    { "௪", "நான்கு" },
    { "௫", "ஐந்து" },
    { "௬", "ஆறு" },
    { "௭", "ஏழு" },
    { "௮", "எட்டு" },
    { "௯", "ஒன்பது" },
    { NULL, NULL },
};

static const char * const indic_tam_two_digit[101][4] =
{
    { "௧", "௦", "பத்து", NULL },
    { "௧", "௧", "பதினொன்று", NULL },
    { "௧", "௨", "பன்னிரண்டு", NULL },
    { "௧", "௩", "பதின்மூன்று", NULL },
    { "௧", "௪", "பதினான்கு", NULL },
    { "௧", "௫", "பதினைந்து", NULL },
    { "௧", "௬", "பதினாறு", NULL },
    { "௧", "௭", "பதினேழு", NULL },
    { "௧", "௮", "பதினெட்டு", NULL },
    { "௧", "௯", "பத்தொன்பது", NULL },
    { "௨", "௦", "இருபது", NULL },
    { "௨", "௧", "இருபத்தொன்ற", NULL },
    { "௨", "௨", "இருபத்திரண்டு", NULL },
    { "௨", "௩", "இருபத்துமூன்று", NULL },
    { "௨", "௪", "இருபத்துநான்கு", NULL },
    { "௨", "௫", "இருபத்தைந்து", NULL },
    { "௨", "௬", "இருபத்தாறு", NULL },
    { "௨", "௭", "இருபத்தேழு", NULL },
    { "௨", "௮", "இருபத்தெட்டு", NULL },
    { "௨", "௯", "இருபத்தொன்பது", NULL },
    { "௩", "௦", "முப்பது", NULL },
    { "௩", "௧", "முப்பத்தொன்று", NULL },
    { "௩", "௨", "முப்பத்திரண்டு", NULL },
    { "௩", "௩", "முப்பத்துமூன்று", NULL },
    { "௩", "௪", "முப்பத்துநான்கு", NULL },
    { "௩", "௫", "முப்பத்தைந்து", NULL },
    { "௩", "௬", "முப்பத்தாறு", NULL },
    { "௩", "௭", "முப்பத்தேழு", NULL },
    { "௩", "௮", "முப்பத்தெட்டு", NULL },
    { "௩", "௯", "முப்பத்தொன்பது", NULL },
    { "௪", "௦", "நாற்பது", NULL },
    { "௪", "௧", "நாற்பத்தொன்று", NULL },
    { "௪", "௨", "நாற்பத்திரண்டு", NULL },
    { "௪", "௩", "நாற்பத்துமூன்று", NULL },
    { "௪", "௪", "நாற்பத்துநான்கு", NULL },
    { "௪", "௫", "நாற்பத்தைந்து", NULL },
    { "௪", "௬", "நாற்பத்தாறு", NULL },
    { "௪", "௭", "நாற்பத்தேழு", NULL },
    { "௪", "௮", "நாற்பத்தெட்டு", NULL },
    { "௪", "௯", "நாற்பத்தொன்பது", NULL },
    { "௫", "௦", "ஐம்பது", NULL },
    { "௫", "௧", "ஐம்பத்தொன்று", NULL },
    { "௫", "௨", "ஐம்பத்திரண்டு", NULL },
    { "௫", "௩", "ஐம்பத்துமூன்று", NULL },
    { "௫", "௪", "ஐம்பத்துநான்கு", NULL },
    { "௫", "௫", "ஐம்பத்தைந்து", NULL },
    { "௫", "௬", "ஐம்பத்தாறு", NULL },
    { "௫", "௭", "ஐம்பத்தேழு", NULL },
    { "௫", "௮", "ஐம்பத்தெட்டு", NULL },
    { "௫", "௯", "ஐம்பத்தொன்பது", NULL },
    { "௬", "௦", "அறுபது", NULL },
    { "௬", "௧", "அறுபத்தொன்று", NULL },
    { "௬", "௨", "அறுபத்திரண்டு", NULL },
    { "௬", "௩", "அறுபத்துமூன்று", NULL },
    { "௬", "௪", "அறுபத்துநான்கு", NULL },
    { "௬", "௫", "அறுபத்தைந்து", NULL },
    { "௬", "௬", "அறுபத்தாறு", NULL },
    { "௬", "௭", "அறுபத்தேழு", NULL },
    { "௬", "௮", "அறுபத்தெட்டு", NULL },
    { "௬", "௯", "அறுபத்தொன்பது", NULL },
    { "௭", "௦", "எழுபது", NULL },
    { "௭", "௧", "எழுபத்தொன்று", NULL },
    { "௭", "௨", "எழுபத்திரண்டு", NULL },
    { "௭", "௩", "எழுபத்துமூன்று", NULL },
    { "௭", "௪", "எழுபத்துநான்கு", NULL },
    { "௭", "௫", "எழுபத்தைந்து", NULL },
    { "௭", "௬", "எழுபத்தாறு", NULL },
    { "௭", "௭", "எழுபத்தேழு", NULL },
    { "௭", "௮", "எழுபத்தெட்டு", NULL },
    { "௭", "௯", "எழுபத்தொன்பது", NULL },
    { "௮", "௦", "எண்பது", NULL },
    { "௮", "௧", "எண்பத்தொன்று", NULL },
    { "௮", "௨", "எண்பத்திரண்டு", NULL },
    { "௮", "௩", "எண்பத்துமூன்று", NULL },
    { "௮", "௪", "எண்பத்துநான்கு", NULL },
    { "௮", "௫", "எண்பத்தைந்து", NULL },
    { "௮", "௬", "எண்பத்தாறு", NULL },
    { "௮", "௭", "எண்பத்தேழு", NULL },
    { "௮", "௮", "எண்பத்தெட்டு", NULL },
    { "௮", "௯", "எண்பத்தொன்பது", NULL },
    { "௯", "௦", "தொண்ணூறு", NULL },
    { "௯", "௧", "தொண்ணூற்றொன்று", NULL },
    { "௯", "௨", "தொண்ணூற்றிரண்டு", NULL },
    { "௯", "௩", "தொண்ணூற்றுமூன்று", NULL },
    { "௯", "௪", "தொண்ணூற்றுநான்கு", NULL },
    { "௯", "௫", "தொண்ணூற்றைந்து", NULL },
    { "௯", "௬", "தொண்ணூற்றாறு", NULL },
    { "௯", "௭", "தொண்ணூற்றேழு", NULL },
    { "௯", "௮", "தொண்ணூற்றெட்டு", NULL },
    { "௯", "௯", "தொண்ணூற்றொன்பது", NULL },
    { NULL, NULL },
};

const static indic_num_table tam_num_table = {
    "tam",
    &indic_tam_digit,
    &indic_tam_two_digit,
    "நூறு",   /* hundred */
    "ஆயிரம்", /* thousand */
    "லட்சம்",  /* lakh */
    "கோடி", /* crore */
    "point"  /* should be native word for "point" */
};


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif

    
