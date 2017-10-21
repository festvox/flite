/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 1999                             */
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
/*               Date:  December 1999                                    */
/*************************************************************************/
/*                                                                       */
/*  feature-values lists                                                 */
/*                                                                       */
/*************************************************************************/
#ifndef _CST_FEATURES_H__
#define _CST_FEATURES_H__

#include "cst_alloc.h"
#include "cst_val.h"
#include "cst_string.h"

typedef struct cst_featvalpair_struct {
    const char *name;
    cst_val *val;
    struct cst_featvalpair_struct *next;
} cst_featvalpair;

typedef struct cst_features_struct {
    struct cst_featvalpair_struct *head;
    cst_alloc_context ctx;
    cst_val *owned_strings; /* fnames that are owned by this struct */

    /* Link to other cst_features that we search too */
    const struct cst_features_struct *linked; 
} cst_features;

/* Constructor functions */
cst_features *new_features(void);
cst_features *new_features_local(cst_alloc_context ctx);
void delete_features(cst_features *f);

/* Accessor functions */
int feat_int(const cst_features *f, const char *name);
float feat_float(const cst_features *f, const char *name);
const char *feat_string(const cst_features *f, const char *name);
const cst_val *feat_val(const cst_features *f, const char *name);

int get_param_int(const cst_features *f, const char *name,int def);
float get_param_float(const cst_features *f, const char *name, float def);
const char *get_param_string(const cst_features *f, const char *name, const char *def);
const cst_val *get_param_val(const cst_features *f, const char *name, cst_val *def);

/* Setting functions */
void feat_set_int(cst_features *f, const char *name, int v);
void feat_set_float(cst_features *f, const char *name, float v);
void feat_set_string(cst_features *f, const char *name, const char *v);
void feat_set(cst_features *f, const char *name,const cst_val *v);

int feat_remove(cst_features *f,const char *name);
int feat_present(const cst_features *f,const char *name);
int feat_length(const cst_features *f);

/* Feature names are expected to be literals -- its *much* faster to do */
/* look ups and less alloc/frees are necesssary.  Almosts always this is */
/* fine, but when you are making up new fnames, you can get a copy of the */
/* string that will be deleted when the cst_features is deleted */
const char *feat_own_string(cst_features *f,const char *name);

CST_VAL_USER_TYPE_DCLS(features,cst_features)

int feat_copy_into(const cst_features *from,cst_features *to);
/* Link FROM into TO so FROM's features will be searched after TO's features */
int feat_link_into(const cst_features *from,cst_features *to);
/* For debugging */
int cst_feat_print(cst_file fd,const cst_features *f);

#endif
