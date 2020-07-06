/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                      Copyright (c) 2010-2017                          */
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
#include "cst_string.h"
#include "cst_cg_map.h"

const char * const cg_voice_header_string = "CMU_FLITE_CG_VOXDATA-v2.0";

int cst_cg_read_header(cst_file fd)
{
    char header[200];
    unsigned int n;
    int endianness;

    n = cst_fread(fd,header,sizeof(char),cst_strlen(cg_voice_header_string)+1);

    if (n < cst_strlen(cg_voice_header_string)+1)
        return -1;

    if (!cst_streq(header,cg_voice_header_string))
        return -1;

    cst_fread(fd,&endianness,sizeof(int),1); /* for byte order check */
    if (endianness != cst_endian_loc)
        return CST_CG_BYTESWAPPED_VOICE; /* dumped with other byte order */
  
    return 0;
}

char *cst_read_string(cst_file fd, int bs)
{
    int numbytes;

    return (char *)cst_read_padded(fd,&numbytes,bs);
}

cst_cg_db *cst_cg_load_db(cst_voice *vox,cst_file fd, int bs /* byteswapped */)
{
    cst_cg_db* db = cst_alloc(cst_cg_db,1);
    int i;

    db->freeable = 1;  /* somebody can free this if they want */

    db->name = cst_read_string(fd,bs);
    db->types = (const char**)cst_read_db_types(fd,bs);

    db->num_types = cst_read_int(fd,bs);
    db->sample_rate = cst_read_int(fd,bs);
    db->f0_mean = cst_read_float(fd,bs);
    db->f0_stddev = cst_read_float(fd,bs);

    db->num_f0_models = get_param_int(vox->features,"num_f0_models",1);
    db->f0_trees = cst_alloc(const cst_cart **,db->num_f0_models);
    for (i=0; i<db->num_f0_models; i++)
        db->f0_trees[i] = (const cst_cart**) cst_read_tree_array(fd,bs);

    db->model_shape = get_param_int(vox->features,"model_shape",
                                    CST_CG_MODEL_SHAPE_BASE_MINRANGE);
    db->num_param_models = get_param_int(vox->features,"num_param_models",1);
    db->param_trees = cst_alloc(const cst_cart **,db->num_param_models);
    for (i=0; i<db->num_param_models; i++)
        db->param_trees[i] = (const cst_cart **) cst_read_tree_array(fd,bs);

    db->spamf0 = cst_read_int(fd,bs);
    if (db->spamf0)
    {
        db->spamf0_accent_tree = cst_read_tree(fd,bs);
        db->spamf0_phrase_tree = cst_read_tree(fd,bs);
    }

    db->num_channels = cst_alloc(int,db->num_param_models);
    db->num_frames = cst_alloc(int,db->num_param_models);
    db->model_vectors = cst_alloc(const unsigned short **,db->num_param_models);
    for (i=0; i<db->num_param_models; i++)
    {
        db->num_channels[i] = cst_read_int(fd,bs);
        db->num_frames[i] = cst_read_int(fd,bs);
        db->model_vectors[i] =
            (const unsigned short **)cst_read_2d_ushort_array(fd,bs);
    }
    /* In voices that were built before, they might have NULLs as the */
    /* the vectors rather than a real model, so adjust the num_param_models */
    /* accordingly -- this wont cause a leak as there is no alloc'd memory */
    /* in the later unset vectors */
    for (i=0; i<db->num_param_models; i++)
    {
        if (db->model_vectors[i] == NULL)
            break;
    }
    db->num_param_models = i;

    if (db->spamf0)
    {
        db->num_channels_spamf0_accent = cst_read_int(fd,bs);
        db->num_frames_spamf0_accent = cst_read_int(fd,bs);
        db->spamf0_accent_vectors = 
            (const float * const *)cst_read_2d_float_array(fd,bs);
    }

    db->model_min = (const float *)cst_read_float_array(fd,bs);
    db->model_range = (const float *)cst_read_float_array(fd,bs);

    if (db->model_shape > CST_CG_MODEL_SHAPE_BASE_MINRANGE)
    {   /* there is a qtable if shape > 1 */
        db->qtable = cst_alloc(const float **,db->num_param_models);
        for (i=0; i<db->num_param_models; i++)
            db->qtable[i] =
                (const float **)cst_read_2d_float_array(fd,bs);
    }

    db->frame_advance = cst_read_float(fd,bs);

    db->num_dur_models = get_param_int(vox->features,"num_dur_models",1);
    db->dur_stats = cst_alloc(const dur_stat **,db->num_dur_models);
    db->dur_cart = cst_alloc(const cst_cart *,db->num_dur_models);

    for (i=0; i<db->num_dur_models; i++)
    {
        db->dur_stats[i] = (const dur_stat **)cst_read_dur_stats(fd,bs);
        db->dur_cart[i] = (const cst_cart *)cst_read_tree(fd,bs);
    }

    db->phone_states = 
        (const char * const * const *)cst_read_phone_states(fd,bs);

    db->do_mlpg = cst_read_int(fd,bs);
    db->dynwin = cst_read_float_array(fd,bs);
    db->dynwinsize = cst_read_int(fd,bs);

    db->mlsa_alpha = cst_read_float(fd,bs);
    db->mlsa_beta = cst_read_float(fd,bs);

    db->multimodel = cst_read_int(fd,bs);
    db->mixed_excitation = cst_read_int(fd,bs);

    db->ME_num = cst_read_int(fd,bs);
    db->ME_order = cst_read_int(fd,bs);
    db->me_h = (const double * const *)cst_read_2d_double_array(fd,bs);
    
    db->spamf0 = cst_read_int(fd,bs); /* yes, twice, its above too */
    db->gain = cst_read_float(fd,bs);

    /* If this is "grapheme" voice, we will have phoneset and char_map */

    return db;
  
}

void cst_cg_free_db(cst_file fd, cst_cg_db *db)
{
    /* Only gets called when this isn't populated : I think */ 
    cst_free(db);
}

void *cst_read_padded(cst_file fd, int *numbytes, int bs)
{
    void* ret;
    int n; 

    *numbytes = cst_read_int(fd,bs);
    ret = (void *)cst_alloc(char,*numbytes);
    n = cst_fread(fd,ret,sizeof(char),*numbytes);
    if (n != (*numbytes))
    {
        cst_free(ret);
        return NULL;
    }
    return ret;
}

char **cst_read_db_types(cst_file fd, int bs)
{
    char** types;
    int numtypes;
    int i;

    numtypes = cst_read_int(fd,bs);
    types = cst_alloc(char*,numtypes+1);
  
    for(i=0;i<numtypes;i++)
    {
        types[i] = cst_read_string(fd,bs);
    }
    types[i] = 0;
  
    return types;
}

cst_cart_node* cst_read_tree_nodes(cst_file fd,int bs)
{   
    cst_cart_node* nodes;
    int i, num_nodes;
    short vtype;
    char *str;

    num_nodes = cst_read_int(fd,bs);
    nodes = cst_alloc(cst_cart_node,num_nodes+1);

    for (i=0; i<num_nodes; i++)
    {
        cst_fread(fd,&nodes[i].feat,sizeof(char),1);
        cst_fread(fd,&nodes[i].op,sizeof(char),1);
        cst_fread(fd,&nodes[i].no_node,sizeof(short),1);
        if (bs) nodes[i].no_node = SWAPSHORT(nodes[i].no_node);
        cst_fread(fd,&vtype,sizeof(short),1);
        if (bs) vtype = SWAPSHORT(vtype);
        if (vtype == CST_VAL_TYPE_STRING)
        {
            str = cst_read_string(fd,bs);
            nodes[i].val = string_val(str);
            cst_free(str);
        }
        else if (vtype == CST_VAL_TYPE_INT)
            nodes[i].val = int_val(cst_read_int(fd,bs));
        else if (vtype == CST_VAL_TYPE_FLOAT)
            nodes[i].val = float_val(cst_read_float(fd,bs));
        else
            nodes[i].val = int_val(cst_read_int(fd,bs));
    }
    nodes[i].val = NULL;

    return nodes;
}

char** cst_read_tree_feats(cst_file fd,int bs)
{
    char** feats;
    int numfeats;
    int i;

    numfeats = cst_read_int(fd,bs);
    feats = cst_alloc(char *,numfeats+1);

    for(i=0;i<numfeats;i++)
        feats[i] = cst_read_string(fd,bs);
    feats[i] = 0;
  
    return feats;
}

cst_cart* cst_read_tree(cst_file fd,int bs)
{
    cst_cart* tree;

    tree = cst_alloc(cst_cart,1);
    tree->rule_table = cst_read_tree_nodes(fd,bs);  
    tree->feat_table = (const char * const *)cst_read_tree_feats(fd,bs);

    return tree;
}

cst_cart** cst_read_tree_array(cst_file fd,int bs)
{
    cst_cart** trees = NULL;
    int numtrees;
    int i;

    numtrees = cst_read_int(fd,bs);
    if (numtrees > 0)
    {
        trees = cst_alloc(cst_cart *,numtrees+1);

        for(i=0;i<numtrees;i++)
            trees[i] = cst_read_tree(fd,bs);
        trees[i] = 0;
    }

    return trees; 
}

#if 0
void* cst_read_array(cst_file fd,int bs)
{
    int temp;
    void* ret;
    ret = cst_read_padded(fd,&temp,bs);
    return ret;
}
#endif

float *cst_read_float_array(cst_file fd,int bs)
{
    unsigned int i;
    int bytecount;
    float* ret;

    ret = (float *)cst_read_padded(fd,&bytecount,bs);
    if (bs)
        for (i=0; i<bytecount/sizeof(float); i++)
            swapfloat(&ret[i]);
    return ret;
}

unsigned short *cst_read_ushort_array(cst_file fd,int bs)
{
    unsigned int i;
    int bytecount;
    unsigned short* ret;

    ret = (unsigned short *)cst_read_padded(fd,&bytecount,bs);
    if (bs)
        for (i=0; i<bytecount/sizeof(unsigned short); i++)
            ret[i] = SWAPSHORT(ret[i]);
    return ret;
}

double *cst_read_double_array(cst_file fd,int bs)
{
    unsigned int i;
    int bytecount;
    double* ret;

    ret = (double *)cst_read_padded(fd,&bytecount,bs);
    if (bs)
        for (i=0; i<bytecount/sizeof(double); i++)
            swapdouble(&ret[i]);
    return ret;
}

float** cst_read_2d_float_array(cst_file fd,int bs)
{
    int numrows;
    int i;
    float** arrayrows = NULL;

    numrows = cst_read_int(fd,bs);
    if (numrows > 0)
    {
        arrayrows = cst_alloc(float *,numrows);
        for(i=0;i<numrows;i++)
            arrayrows[i] = cst_read_float_array(fd,bs);
    }

    return arrayrows; 
}

unsigned short** cst_read_2d_ushort_array(cst_file fd,int bs)
{
    int numrows;
    int i;
    unsigned short** arrayrows = NULL;

    numrows = cst_read_int(fd,bs);
    if (numrows > 0)
    {
        arrayrows = cst_alloc(unsigned short *,numrows);
        for(i=0;i<numrows;i++)
            arrayrows[i] = cst_read_ushort_array(fd,bs);
    }

    return arrayrows; 
}

double** cst_read_2d_double_array(cst_file fd,int bs)
{
    int numrows;
    int i;
    double** arrayrows = NULL;

    numrows = cst_read_int(fd,bs);
    if (numrows > 0)
    {
        arrayrows = cst_alloc(double *,numrows);
        for(i=0;i<numrows;i++)
            arrayrows[i] = cst_read_double_array(fd,bs);
    }

    return arrayrows; 
}

dur_stat** cst_read_dur_stats(cst_file fd,int bs)
{
    int numstats;
    int i;
    dur_stat** ds;

    numstats = cst_read_int(fd,bs);
    ds = cst_alloc(dur_stat *,(1+numstats));

    /* load structuer values */
    for(i=0;i<numstats;i++)
    {
        ds[i] = cst_alloc(dur_stat,1);
        ds[i]->mean = cst_read_float(fd,bs);
        ds[i]->stddev = cst_read_float(fd,bs);
        ds[i]->phone = cst_read_string(fd,bs);
    }
    ds[i] = NULL;

    return ds;
}

char*** cst_read_phone_states(cst_file fd,int bs)
{
    int i,j,count1,count2;
    char*** ps;

    count1 = cst_read_int(fd,bs);
    ps = cst_alloc(char **,count1+1);
    for(i=0;i<count1;i++)
    {
        count2 = cst_read_int(fd,bs);
        ps[i] = cst_alloc(char *,count2+1);
        for(j=0;j<count2;j++)
	{
            ps[i][j]=cst_read_string(fd,bs);
	}
        ps[i][j] = 0;
    }
    ps[i] = 0;

    return ps;
}

void cst_read_voice_feature(cst_file fd,char** fname, char** fval, int bs)
{
    *fname = cst_read_string(fd,bs);
    *fval = cst_read_string(fd,bs);
}

int cst_read_int(cst_file fd,int byteswapped)
{
    int val;
    int n;

    n = cst_fread(fd,&val,sizeof(int),1);
    if (n != 1) return 0;
    if (byteswapped) val = SWAPINT(val);
    return val;
}

float cst_read_float(cst_file fd, int byteswapped)
{
    float val;
    int n;

    n = cst_fread(fd,&val,sizeof(float),1);
    if (n != 1)
        return 0;
    if (byteswapped) swapfloat(&val);
    return val;
}
