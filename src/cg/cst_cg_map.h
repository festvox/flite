/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2010                            */
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
/*               Date:  April 2010                                       */
/*************************************************************************/
/*                                                                       */
/*  A clustergen generic voice, that can load from a file                */
/*                                                                       */
/*************************************************************************/
#ifndef __CST_CG_MAP_H
#define __CST_CG_MAP_H

#include "cst_file.h"
#include "cst_cg.h"
#include "cst_cart.h"
#include "cst_val.h"
#include "cst_synth.h"

/* If voice to be read was dumped on a platform byteswapped from this one */
#define CST_CG_BYTESWAPPED_VOICE 27

int cst_cg_read_header(cst_file fd);

cst_cg_db *cst_cg_load_db(cst_voice *vox,cst_file fd, int bs);
void cst_cg_free_db(cst_file fd,cst_cg_db*);

char *cst_read_string(cst_file fd, int bs);
void* cst_read_padded(cst_file fd, int*nb, int bs); 
char** cst_read_db_types(cst_file fd, int bs);

cst_cart_node* cst_read_tree_nodes(cst_file fd, int bs);
char** cst_read_tree_feats(cst_file fd, int bs);
cst_cart* cst_read_tree(cst_file fd, int bs);
cst_cart** cst_read_tree_array(cst_file fd, int bs);

float* cst_read_float_array(cst_file fd, int bs);
double* cst_read_double_array(cst_file fd, int bs);
unsigned short* cst_read_ushort_array(cst_file fd, int bs);
float** cst_read_2d_float_array(cst_file fd, int bs);
double** cst_read_2d_double_array(cst_file fd, int bs);
unsigned short** cst_read_2d_ushort_array(cst_file fd, int bs);

dur_stat** cst_read_dur_stats(cst_file fd, int bs);

char*** cst_read_phone_states(cst_file fd, int bs);

void cst_read_voice_feature(cst_file fd,char** fname, char** fval, int bs);
int cst_read_int(cst_file fd, int bs);
float cst_read_float(cst_file fd, int bs);

extern const char * const cg_voice_header_string;

#endif
