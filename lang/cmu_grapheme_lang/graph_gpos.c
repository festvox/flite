/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2001                            */
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
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2022                                     */
/*************************************************************************/
/*  This is a guess PoS tagger for grapheme languages                    */
/*                                                                       */
/*  But its not!                                                         */
/*                                                                       */
/*  The default Festival Grapheme build falls back on the English gpos   */
/*  lookup mechanism to check for function/content words, thus the       */
/*  flite grapheme front end *must* have the same fall back position     */
/*  otherwise the gpos feature will be different.  After checking things */
/*  it does get used, and not just for punc vs not punc so here is       */
/*  an inappropriate list of words for non-English languages but sorry   */
/*  has to be here.  We could use the actual us_gpos but that is in a    */
/*  different library so we'd need to modify the link command so we just */
/*  copy it in all its embarassing shame :-(                             */
/*                                                                       */
/*  The real bug fix for this is remove us_gpos from the Festival        */
/*  grapheme build and rebuild all 700 wilderness voices                 */
/*  Additionally you could learn a new unknown language gpos and us it   */
/*  for phrase prediction, but the gpos table would need to in the       */
/*  the .flitevox file, not the language independent graph_lang lib      */
/*                                                                       */
/*************************************************************************/

#include "cst_val.h"

DEF_STATIC_CONST_VAL_STRING(gr_gpos_in,"in");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_of,"of");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_for,"for");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_on,"on");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_that,"that");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_with,"with");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_by,"by");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_at,"at");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_from,"from");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_as,"as");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_if,"if");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_against,"against");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_about,"about");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_before,"before");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_because,"because");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_under,"under");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_after,"after");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_over,"over");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_into,"into");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_while,"while");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_without,"without");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_through,"through");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_new,"new"); /* ??? */
DEF_STATIC_CONST_VAL_STRING(gr_gpos_between,"between");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_among,"among");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_until,"until");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_per,"per");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_up,"up");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_down,"down");

static const cst_val * const gr_gpos_in_list[] = {
    (cst_val *)&gr_gpos_in,
    (cst_val *)&gr_gpos_of,
    (cst_val *)&gr_gpos_for,
    (cst_val *)&gr_gpos_in,
    (cst_val *)&gr_gpos_on,
    (cst_val *)&gr_gpos_that,
    (cst_val *)&gr_gpos_with,
    (cst_val *)&gr_gpos_by,
    (cst_val *)&gr_gpos_at,
    (cst_val *)&gr_gpos_from,
    (cst_val *)&gr_gpos_as,
    (cst_val *)&gr_gpos_if,
    (cst_val *)&gr_gpos_that,
    (cst_val *)&gr_gpos_against,
    (cst_val *)&gr_gpos_about,
    (cst_val *)&gr_gpos_before,
    (cst_val *)&gr_gpos_because,
    (cst_val *)&gr_gpos_under,
    (cst_val *)&gr_gpos_after,
    (cst_val *)&gr_gpos_over,
    (cst_val *)&gr_gpos_into,
    (cst_val *)&gr_gpos_while,
    (cst_val *)&gr_gpos_without,
    (cst_val *)&gr_gpos_through,
    (cst_val *)&gr_gpos_new,
    (cst_val *)&gr_gpos_between,
    (cst_val *)&gr_gpos_among,
    (cst_val *)&gr_gpos_until,
    (cst_val *)&gr_gpos_per,
    (cst_val *)&gr_gpos_up,
    (cst_val *)&gr_gpos_down,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_to,"to");

static const cst_val * const gr_gpos_to_list[] = {
    (cst_val *)&gr_gpos_to,
    (cst_val *)&gr_gpos_to,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_det,"det");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_the,"the");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_a,"a");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_an,"an");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_some,"some");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_this,"this");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_each,"each");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_another,"another");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_those,"those");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_every,"every");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_all,"all");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_any,"any");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_these,"these");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_both,"both");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_neither,"neither");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_no,"no");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_many,"many");

static const cst_val * const gr_gpos_det_list[] = {
    (cst_val *)&gr_gpos_det,
    (cst_val *)&gr_gpos_the,
    (cst_val *)&gr_gpos_a,
    (cst_val *)&gr_gpos_an,
    (cst_val *)&gr_gpos_no,
    (cst_val *)&gr_gpos_some,
    (cst_val *)&gr_gpos_this,
    (cst_val *)&gr_gpos_each,
    (cst_val *)&gr_gpos_another,
    (cst_val *)&gr_gpos_those,
    (cst_val *)&gr_gpos_every,
    (cst_val *)&gr_gpos_all,
    (cst_val *)&gr_gpos_any,
    (cst_val *)&gr_gpos_these,
    (cst_val *)&gr_gpos_both,
    (cst_val *)&gr_gpos_neither,
    (cst_val *)&gr_gpos_no,
    (cst_val *)&gr_gpos_many,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_md,"md");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_will,"will");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_may,"may");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_would,"would");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_can,"can");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_could,"could");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_should,"should");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_must,"must");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_ought,"ought");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_might,"might");

static const cst_val * const gr_gpos_md_list[] = {
    (cst_val *)&gr_gpos_md,
    (cst_val *)&gr_gpos_will,
    (cst_val *)&gr_gpos_may,
    (cst_val *)&gr_gpos_would,
    (cst_val *)&gr_gpos_can,
    (cst_val *)&gr_gpos_could,
    (cst_val *)&gr_gpos_should,
    (cst_val *)&gr_gpos_must,
    (cst_val *)&gr_gpos_ought,
    (cst_val *)&gr_gpos_might,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_cc,"cc");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_and,"and");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_but,"but");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_or,"or");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_plus,"plus");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_yet,"yet");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_nor,"nor");

static const cst_val * const gr_gpos_cc_list[] = {
    (cst_val *)&gr_gpos_cc,
    (cst_val *)&gr_gpos_and,
    (cst_val *)&gr_gpos_but,
    (cst_val *)&gr_gpos_or,
    (cst_val *)&gr_gpos_plus,
    (cst_val *)&gr_gpos_yet,
    (cst_val *)&gr_gpos_nor,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_wp,"wp");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_who,"who");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_what,"what");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_where,"where");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_how,"how");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_when,"when");

static const cst_val * const gr_gpos_wp_list[] = {
    (cst_val *)&gr_gpos_wp,
    (cst_val *)&gr_gpos_who,
    (cst_val *)&gr_gpos_what,
    (cst_val *)&gr_gpos_where,
    (cst_val *)&gr_gpos_how,
    (cst_val *)&gr_gpos_when,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_pps,"pps");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_her,"her");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_his,"his");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_their,"their");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_its,"its");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_our,"our");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_mine,"mine");

static const cst_val * const gr_gpos_pps_list[] = {
    (cst_val *)&gr_gpos_pps,
    (cst_val *)&gr_gpos_her,
    (cst_val *)&gr_gpos_his,
    (cst_val *)&gr_gpos_their,
    (cst_val *)&gr_gpos_its,
    (cst_val *)&gr_gpos_our,
    (cst_val *)&gr_gpos_mine,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_aux,"aux");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_is,"is");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_am,"am");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_are,"are");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_was,"was");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_were,"were");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_has,"has");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_have,"have");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_had,"had");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_be,"be");

static const cst_val * const gr_gpos_aux_list[] = {
    (cst_val *)&gr_gpos_aux,
    (cst_val *)&gr_gpos_is,
    (cst_val *)&gr_gpos_am,
    (cst_val *)&gr_gpos_are,
    (cst_val *)&gr_gpos_was,
    (cst_val *)&gr_gpos_were,
    (cst_val *)&gr_gpos_has,
    (cst_val *)&gr_gpos_have,
    (cst_val *)&gr_gpos_had,
    (cst_val *)&gr_gpos_be,
    0 };

DEF_STATIC_CONST_VAL_STRING(gr_gpos_punc,"punc");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_dot,".");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_comma,",");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_colon,":");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_semicolon,";");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_dquote,"\"");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_squote,"'");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_leftparen,"(");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_qmark,"?");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_rightparen,")");
DEF_STATIC_CONST_VAL_STRING(gr_gpos_emark,"!");

static const cst_val * const gr_gpos_punc_list[] = {
    (cst_val *)&gr_gpos_punc,
    (cst_val *)&gr_gpos_dot,
    (cst_val *)&gr_gpos_comma,
    (cst_val *)&gr_gpos_colon,
    (cst_val *)&gr_gpos_semicolon,
    (cst_val *)&gr_gpos_dquote,
    (cst_val *)&gr_gpos_squote,
    (cst_val *)&gr_gpos_leftparen,
    (cst_val *)&gr_gpos_qmark,
    (cst_val *)&gr_gpos_rightparen,
    (cst_val *)&gr_gpos_emark,
    0 };

const cst_val * const * const gr_gpos[] = {
    gr_gpos_in_list,
    gr_gpos_to_list,
    gr_gpos_det_list,
    gr_gpos_md_list,
    gr_gpos_cc_list,
    gr_gpos_wp_list,
    gr_gpos_pps_list,
    gr_gpos_aux_list,
    gr_gpos_punc_list,
    0 };

    
	
