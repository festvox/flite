/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2013                             */
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
/*             Author:  Alok Parlikar (aup@cs.cmu.edu)                   */
/*               Date:  January 2013                                     */
/*************************************************************************/
/*                                                                       */
/*  Multiple-thread test, do 50 synthesis calls over OMP_NUM_THREADS     */
/*                                                                       */
/*  This particular test uses OMP to do the threads                      */
/*                                                                       */
/*************************************************************************/
#include <stdio.h>
#include <omp.h>

#include <flite.h>

cst_voice *voice;

cst_voice *register_cmu_us_slt(const char *voxdir);

cst_val *flite_set_voice_list(const char *voxdir)
{
    flite_voice_list = cons_val(voice_val(register_cmu_us_slt(voxdir)),flite_voice_list);
    flite_voice_list = val_reverse(flite_voice_list);
    return flite_voice_list;
}

void init() {
  flite_init();
  flite_set_voice_list(NULL);
  voice = flite_voice_select("cmu_us_slt");
  
}

float synth_text(char* text) {
  float dur;
  dur = flite_text_to_speech(text, voice, "none");
  return dur;
}

int main() {
  init();
  int i;
#pragma omp parallel for 
    for (i=0; i<50; i++) {
      printf("%d %d %f\n", omp_get_thread_num(), i, synth_text("Hello"));
    }
    
    return 0;
}
      
