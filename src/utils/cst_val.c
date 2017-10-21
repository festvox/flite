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
/*  Typed values                                                          */
/*                                                                       */
/*************************************************************************/
#include "cst_math.h"
#include "cst_file.h"
#include "cst_val.h"
#include "cst_string.h"
#include "cst_tokenstream.h"

static cst_val *new_val()
{
    return cst_alloc(struct cst_val_struct,1);
}

cst_val *int_val(int i)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_INT;
    CST_VAL_INT(v) = i;
    return v;
}
    
cst_val *float_val(float f)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_FLOAT;
    CST_VAL_FLOAT(v) = f;
    return v;
}

cst_val *string_val(const char *s)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_STRING;
    /* would be nice to note if this is a deletable string or not */
    CST_VAL_STRING_LVAL(v) = cst_strdup(s);
    return v;
}

cst_val *cons_val(const cst_val *a, const cst_val *b)
{
    cst_val *v = new_val();
    CST_VAL_CAR(v)=((!a || cst_val_consp(a)) ? 
		    (cst_val *)(void *)a:val_inc_refcount(a));
    CST_VAL_CDR(v)=((!b || cst_val_consp(b)) ? 
		    (cst_val *)(void *)b:val_inc_refcount(b));
    return v;
}

cst_val *val_new_typed(int type,void *vv)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = type;
    CST_VAL_VOID(v) = vv;
    return v;
}

void delete_val_list(cst_val *v)
{
    if (v)
    {
	if (cst_val_consp(v))
	{
	    delete_val_list(CST_VAL_CDR(v));
	    cst_free(v);
	}
	else
	    delete_val(v);
    }
}

void delete_val(cst_val *v)
{
    if (v)
    {
	if (cst_val_consp(v))
	{
	    delete_val(CST_VAL_CAR(v));
	    delete_val(CST_VAL_CDR(v));
	    cst_free(v);
	}
	else if (val_dec_refcount(v) == 0)
	{
	    if (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING)
		cst_free(CST_VAL_VOID(v));
	    else if (CST_VAL_TYPE(v) >= CST_VAL_TYPE_FIRST_FREE)
            {
                if (cst_val_defs[CST_VAL_TYPE(v)/2].delete_function)
                    (cst_val_defs[CST_VAL_TYPE(v)/2].delete_function)
                        (CST_VAL_VOID(v));
            }
            cst_free(v);
	}
    }
}

/* Accessor functions */
int val_int(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT))
	return CST_VAL_INT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT))
	return (int)CST_VAL_FLOAT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return atoi(CST_VAL_STRING(v));
    else
    {
	cst_errmsg("VAL: tried to access int in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

float val_float(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT))
	return (float)CST_VAL_INT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT))
	return CST_VAL_FLOAT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return cst_atof(CST_VAL_STRING(v));
    else
    {
	cst_errmsg("VAL: tried to access float in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const char *val_string(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return CST_VAL_STRING(v);
    else
    {
	cst_errmsg("VAL: tried to access string in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const cst_val *val_car(const cst_val *v)
{
    if (v && cst_val_consp(v))
	return CST_VAL_CAR(v);
    else
    {
	cst_errmsg("VAL: tried to access car in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const cst_val *val_cdr(const cst_val *v)
{
    if (v && cst_val_consp(v))
	return CST_VAL_CDR(v);
    else
    {
	cst_errmsg("VAL: tried to access cdr in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

void *val_generic(const cst_val *v, int type, const char *stype)
{   /* a generic access function that checks the expected type */
    if (v && CST_VAL_TYPE(v) == type)
	return CST_VAL_VOID(v);
    else
    {
        cst_errmsg("VAL: tried to access %s in %d type val\n",
                       stype,
                       (v ? CST_VAL_TYPE(v) : -1));
        cst_error();
    }
    return NULL;
}

void *val_void(const cst_val *v)
{
    /* The scary, do anything function, this shouldn't be called by mortals */
    if ((v == NULL) ||
	(CST_VAL_TYPE(v) == CST_VAL_TYPE_CONS) ||
	(CST_VAL_TYPE(v) == CST_VAL_TYPE_INT) ||
	(CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT))
    {
	cst_errmsg("VAL: tried to access void in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
	return NULL;
    }
    else 
	return CST_VAL_VOID(v);
}

int cst_val_consp(const cst_val *v)
{
    /* To keep a val cell down to 8 bytes we identify non-cons cells  */
    /* with non-zero values in the least significant bit of the first */
    /* address in the cell (this is a standard technique used on Lisp */
    /* machines)                                                      */
#if 0
    void *t;
    int t1;

    /* Hmm this still isn't right (it can be) but this isn't it */
    t = CST_VAL_CAR(v);
    t1 = *(int *)&t;

    if ((t1&0x1) == 0)
	return TRUE;
    else
	return FALSE;
#endif
    const cst_val_atom *t;

    t = (const cst_val_atom *)v;
    if (t->type % 2 == 0)
      return TRUE;
    else
      return FALSE;
}

const cst_val *set_cdr(cst_val *v1, const cst_val *v2)
{
    /* destructive set cdr, be careful you have a pointer to current cdr */
    
    if (!cst_val_consp(v1))
    {
	cst_errmsg("VAL: tried to set cdr of non-consp cell\n");
	cst_error();
	return NULL;
    }
    else
    {
        if (CST_VAL_CDR(v1))
        {
            val_dec_refcount(CST_VAL_CDR(v1));
            val_inc_refcount(v1);
        }
	CST_VAL_CDR(v1) = (cst_val *)v2;
    }
    return v1;
}

const cst_val *set_car(cst_val *v1, const cst_val *v2)
{
    /* destructive set car, be careful you have a pointer to current car */
    
    if (!cst_val_consp(v1))
    {
	cst_errmsg("VAL: tried to set car of non-consp cell\n");
	cst_error();
	return NULL;
    }
    else
    {
	val_dec_refcount(CST_VAL_CAR(v1));
	val_inc_refcount(v1);
	CST_VAL_CAR(v1) = (cst_val *)v2;
    }
    return v1;
}

void val_print(cst_file fd,const cst_val *v)
{
    const cst_val *p;

    if (v == NULL)
	cst_fprintf(fd,"[null]");
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT)
	cst_fprintf(fd,"%d",val_int(v));
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT)
	cst_fprintf(fd,"%f",val_float(v));
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING)
	cst_fprintf(fd,"%s",val_string(v));
    else if (cst_val_consp(v))
    {
	cst_fprintf(fd,"(");
	for (p=v; p; )
	{
	    val_print(fd,val_car(p));
	    p=val_cdr(p);
	    if (p)
		cst_fprintf(fd," ");
            if (p && !cst_val_consp(p))  /* dotted pairs for non-list */
            {                            
                cst_fprintf(fd,". ");
                val_print(fd,p);
                break;
            }
	}
	cst_fprintf(fd,")");
    }
    else 
	cst_fprintf(fd,"[Val %s 0x%p]",
		cst_val_defs[CST_VAL_TYPE(v)/2].name,CST_VAL_VOID(v));
}

cst_val *val_reverse(cst_val *l)
{   /* destructively reverse the list */
    cst_val *n,*np,*nl;
    for (nl=0,n=l; n; nl=n,n=np)
    {
	np=CST_VAL_CDR(n);
	CST_VAL_CDR(n) = nl;
    }
    return nl;
}

cst_val *val_append(cst_val *l1, cst_val *l2)
{
    /* Destructively add l2 to the end of l1 return l1 */
    cst_val *t;

    if (l1 == 0)
	return l2;
    else
    {
	for (t=l1; val_cdr(t); t=CST_VAL_CDR(t));
	CST_VAL_CDR(t) = l2;
	return l1;
    }
}

int val_length(const cst_val *l)
{
    const cst_val *n;
    int i;

    for (i=0,n=l; n; n=val_cdr(n))
	i++;

    return i;
}

int val_equal(const cst_val *v1, const cst_val *v2)
{
    if (v1 == v2)
	return TRUE;  /* its eq so its equal */
    else if (v1 == 0)
	return FALSE;
    else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE(v2))
    {
	if (cst_val_consp(v1))
	    return ((val_equal(val_car(v1),val_car(v2))) &&
		    (val_equal(val_cdr(v1),val_cdr(v2))));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_INT)
	    return (val_int(v1) == val_int(v2));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_FLOAT)
	    return (val_float(v1) == val_float(v2));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_STRING)
	    return (cst_streq(CST_VAL_STRING(v1),CST_VAL_STRING(v2)));
	else 
	    return CST_VAL_VOID(v1) == CST_VAL_VOID(v2);
    }
    else
	return FALSE;
}

int val_less(const cst_val *v1, const cst_val *v2)
{
    return val_float(v1) < val_float(v2);
}

int val_greater(const cst_val *v1,const cst_val *v2)
{
    return val_float(v1) > val_float(v2);
}

int val_member(const cst_val *v1,const cst_val *l)
{
    const cst_val *i;

    for (i=l; i; i=val_cdr(i))
    {
	if (val_equal(val_car(i),v1))
	    return TRUE;
    }
    return FALSE;
}

int val_member_string(const char *v1,const cst_val *l)
{
    const cst_val *i;

    for (i=l; i; i=val_cdr(i))
    {
	if (cst_streq(v1,val_string(val_car(i))))
	    return TRUE;
    }
    return FALSE;
}

cst_val *val_inc_refcount(const cst_val *b)
{
    cst_val *wb;

    /* Well I was lying, they're not really const, but this is the place */
    /* where breaking const is reasonable                              */
    wb = (cst_val *)(void *)b;

    if (CST_VAL_REFCOUNT(wb) == -1) 
	/* or is a cons cell in the text segment, how do I do that ? */
	return wb;
    else if (!cst_val_consp(wb)) /* we don't ref count cons cells */
	CST_VAL_REFCOUNT(wb) += 1;
    return wb;
}

int val_dec_refcount(const cst_val *b)
{
    cst_val *wb;

    wb = (cst_val *)(void *)b;

    if (CST_VAL_REFCOUNT(wb) == -1) 
	/* or is a cons cell in the text segment, how do I do that ? */
	return -1;
    else if (cst_val_consp(wb)) /* we don't ref count cons cells */
	return 0;
    else if (CST_VAL_REFCOUNT(wb) == 0)
    {
	/* Otherwise, trying to free a val outside an
           item/relation/etc has rather the opposite effect from what
           you might have intended... */
	return 0;
    }
    else
    {
	CST_VAL_REFCOUNT(wb) -= 1;
	return 	CST_VAL_REFCOUNT(wb);
    }
}

#ifdef _WIN32
__inline int utf8_sequence_length(char c0)
#else
int utf8_sequence_length(char c0)
#endif
{
    /* Get the expected length of UTF8 sequence given its most */
    /* significant byte */
    return (( 0xE5000000 >> (( c0 >> 3 ) & 0x1E )) & 3 ) + 1;
}

cst_val *cst_utf8_explode(const cst_string *utf8string)
{
    /* Return a list of utf8 characters as strings */
    cst_val *chars=NULL;
  
    const unsigned char *str = (const unsigned char*)utf8string;
    char utf8char[5];
    char c0;
    int charlength;

    while ((c0 = *str))
    {
        charlength = utf8_sequence_length(c0);
        cst_snprintf(utf8char, charlength + 1, "%s", str);
        chars = cons_val(string_val(utf8char),chars);
        str += charlength;
    }
    return val_reverse(chars);
}

static int utf8_ord(const char *utf8_seq) 
{
    unsigned int len;
    int ord;

    unsigned char c0, c1, c2, c3;  /* Potential bytes in the UTF8 symbol */
    c0 = utf8_seq[0];
    len = utf8_sequence_length(c0);

    /* Make sure the string sequence we received matches with the */
    /* expected length, and that the expected length is nonzero.  */
    if ( (len == 0) ||
         (len != strlen(utf8_seq))) {
        return -1;
    }

    if (len == 1) {
        /* ASCII sequence. */
        return c0;
    }

    c1 = utf8_seq[1];
    if (len == 2) {
        ord = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
        if (ord < 0x80)
            return -1;
        return ord;
    }

    c2 = utf8_seq[2];
    if (len == 3) {
        if ((c2 & 0xC0) != 0x80)
            return -1;
        ord = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
        if (ord < 0x800 ||
            (ord >= 0xD800 && ord <= 0xDFFF))
            return -1;
        return ord;
    }

    c3 = utf8_seq[3];
    if (len == 4) {
        if ((c3 & 0xC0) != 0x80)
            return -1;
        ord =
            ((c0 & 0x7) << 18) | ((c1 & 0x3F) << 12) |
            ((c2 & 0x3F) << 6) | (c3 & 0x3F);
        if (ord < 0x10000 || ord > 0x10FFFF)
            return -1;
        return ord;
    }

    return -1;
}

cst_val *cst_utf8_ord(const cst_val *utf8_char) {
  const char *ch=(const char *)val_string(utf8_char);
  return int_val(utf8_ord(ch));
}
int cst_utf8_ord_string(const char *utf8_char)
{
    return utf8_ord(utf8_char);
}

static int utf8_chr(int ord, char* utf8char) 
{
    unsigned int utf8len;
    int i = 0;

    if (ord < 0x80) {
        utf8len = 1;
    } else if (ord < 0x800) {
        utf8len = 2;
    } else if (ord <= 0xFFFF) {
        utf8len = 3;
    } else if (ord <= 0x200000) {
        utf8len = 4;
    } else {
        /* Replace invalid character with FFFD */
        utf8len = 2;
        ord = 0xFFFD;
    }

    i = utf8len;  /* Index into utf8char */
    utf8char[i--] = 0;

    switch (utf8len) {
        /* These fallthrough deliberately */
    case 6:
        utf8char[i--] = (ord | 0x80) & 0xBF;
        ord >>= 6;
    case 5:
        utf8char[i--] = (ord | 0x80) & 0xBF;
        ord >>= 6;
    case 4:
        utf8char[i--] = (ord | 0x80) & 0xBF;
        ord >>= 6;
    case 3:
        utf8char[i--] = (ord | 0x80) & 0xBF;
        ord >>= 6;
    case 2:
        utf8char[i--] = (ord | 0x80) & 0xBF;
        ord >>= 6;
    case 1:
        switch (utf8len) {
        case 0:
        case 1:
            utf8char[i--] = ord;
            break;
        case 2:
            utf8char[i--] = ord | 0xC0;
            break;
        case 3:
            utf8char[i--] = ord | 0xE0;
            break;
        case 4:
            utf8char[i--] = ord | 0xF0;
        }
    }
    return utf8len;
}

cst_val *cst_utf8_chr(const cst_val *ord) {
  char ch[5];
  int utf8len;

  utf8len = utf8_chr(val_int(ord),ch);
  if (utf8len == 0) {
    return 0;
  }

  return string_val(ch);
}

int val_stringp(const cst_val *v)
{
    if (cst_val_consp(v))
        return FALSE;
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING)
        return TRUE;
    else
        return FALSE;
}

const cst_val *val_assoc_string(const char *v1,const cst_val *al)
{
    const cst_val *i;

    for (i=al; i; i=val_cdr(i))
    {
	if (cst_streq(v1,val_string(val_car(val_car(i)))))
	    return val_car(i);
    }
    return NULL;
}

cst_string *cst_implode(const cst_val *sl)
{
    const cst_val *v;
    int l=0;
    char *s;

    for (v=sl; v; v=val_cdr(v))
    {
        if (val_stringp(val_car(v)))
            l += cst_strlen(val_string(val_car(v)));
    }

    s = cst_alloc(cst_string,l+1);

    for (v=sl; v; v=val_cdr(v))
    {
        if (val_stringp(val_car(v)))
            cst_sprintf(s,"%s%s",s,val_string(val_car(v)));

    }

    return s;
}

cst_val *val_readlist_string(const char *str)
{   /* not fully general but a good start */
    cst_tokenstream *ts;
    cst_val *v = NULL;
    const char *p;

    ts = ts_open_string(str,
                        cst_ts_default_whitespacesymbols,
                        "",
                        "",
                        "");

    while (!ts_eof(ts))
    {
        p = ts_get(ts);
        v = cons_val(string_val(p),v);
    }

    ts_close(ts);

    return val_reverse(v);
}



