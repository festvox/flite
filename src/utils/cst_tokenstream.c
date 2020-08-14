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
/*               Date:  July 1999                                        */
/*************************************************************************/
/*                                                                       */
/*  Tokenizer for strings and files                                      */
/*                                                                       */
/*************************************************************************/
#include "cst_tokenstream.h"

const cst_string * const cst_ts_default_whitespacesymbols = " \t\n\r";
const cst_string * const cst_ts_default_singlecharsymbols = "(){}[]";
const cst_string * const cst_ts_default_prepunctuationsymbols = "\"'`({[";
const cst_string * const cst_ts_default_postpunctuationsymbols = "\"'`.,:;!?(){}[]";

#define TS_BUFFER_SIZE 256

static cst_string ts_getc(cst_tokenstream *ts);
static cst_string internal_ts_getc(cst_tokenstream *ts);

static void set_charclass_table(cst_tokenstream *ts)
{
    int i;
    memset(ts->charclass,0,256);  /* zero everything */
    
    for (i=0; ts->p_whitespacesymbols[i]; i++)
	ts->charclass[(unsigned char)ts->p_whitespacesymbols[i]] |= TS_CHARCLASS_WHITESPACE;
    for (i=0; ts->p_singlecharsymbols[i]; i++)
	ts->charclass[(unsigned char)ts->p_singlecharsymbols[i]] |= TS_CHARCLASS_SINGLECHAR;
    for (i=0; ts->p_prepunctuationsymbols[i]; i++)
	ts->charclass[(unsigned char)ts->p_prepunctuationsymbols[i]] |= TS_CHARCLASS_PREPUNCT;
    for (i=0; ts->p_postpunctuationsymbols[i]; i++)
	ts->charclass[(unsigned char)ts->p_postpunctuationsymbols[i]]|=TS_CHARCLASS_POSTPUNCT;
    return;
}

void set_charclasses(cst_tokenstream *ts,
		     const cst_string *whitespace,
		     const cst_string *singlecharsymbols,
		     const cst_string *prepunctuation,
		     const cst_string *postpunctuation)
{
    ts->p_whitespacesymbols = 
	(whitespace ? whitespace : cst_ts_default_whitespacesymbols);
    ts->p_singlecharsymbols = 
    (singlecharsymbols ? singlecharsymbols : cst_ts_default_singlecharsymbols);
    ts->p_prepunctuationsymbols = 
    (prepunctuation ? prepunctuation : cst_ts_default_prepunctuationsymbols);
    ts->p_postpunctuationsymbols =
   (postpunctuation ? postpunctuation : cst_ts_default_postpunctuationsymbols);

    set_charclass_table(ts);
    return;
}

static void extend_buffer(cst_string **buffer,int *buffer_max)
{
    int new_max;
    cst_string *new_buffer;

    new_max = (*buffer_max)+(*buffer_max)/5;
    new_buffer = cst_alloc(cst_string,new_max);
    memmove(new_buffer,*buffer,*buffer_max);
    cst_free(*buffer);
    *buffer = new_buffer;
    *buffer_max = new_max;
}			  

static cst_tokenstream *new_tokenstream(const cst_string *whitespace,
					const cst_string *singlechars,
					const cst_string *prepunct,
					const cst_string *postpunct)
{   /* Constructor function */
    cst_tokenstream *ts = cst_alloc(cst_tokenstream,1);
    ts->fd = NULL;
    ts->file_pos = 0;
    ts->line_number = 0;
    ts->eof_flag = 0;
    ts->string_buffer = NULL;
    ts->token_pos = 0;
    ts->whitespace = cst_alloc(cst_string,TS_BUFFER_SIZE);
    ts->ws_max = TS_BUFFER_SIZE;
    if (prepunct && prepunct[0])
    {
        ts->prepunctuation = cst_alloc(cst_string,TS_BUFFER_SIZE);
        ts->prep_max = TS_BUFFER_SIZE;
    }
    ts->token = cst_alloc(cst_string,TS_BUFFER_SIZE);
    ts->token_max = TS_BUFFER_SIZE;
    if (postpunct && postpunct[0])
    {
        ts->postpunctuation = cst_alloc(cst_string,TS_BUFFER_SIZE);
        ts->postp_max = TS_BUFFER_SIZE;
    }

    set_charclasses(ts,whitespace,singlechars,prepunct,postpunct);
    ts->current_char = 0;

    return ts;
}

void delete_tokenstream(cst_tokenstream *ts)
{
    cst_free(ts->whitespace);
    cst_free(ts->token);
    if (ts->tags) delete_features(ts->tags);
    if (ts->prepunctuation) cst_free(ts->prepunctuation);
    if (ts->postpunctuation) cst_free(ts->postpunctuation);
    cst_free(ts);
}

cst_tokenstream *ts_open(const char *filename,
			 const cst_string *whitespace,
			 const cst_string *singlechars,
			 const cst_string *prepunct,
			 const cst_string *postpunct)
{
    cst_tokenstream *ts = new_tokenstream(whitespace,
					  singlechars,
					  prepunct,
					  postpunct);

#ifndef UNDER_CE
    if (cst_streq("-",filename))
	ts->fd = stdin;
    else
#endif
	ts->fd = cst_fopen(filename,CST_OPEN_READ|CST_OPEN_BINARY);
    ts_getc(ts);

    if (ts->fd == NULL)
    {
	delete_tokenstream(ts);
	return NULL;
    }
    else
	return ts;
}

cst_tokenstream *ts_open_string(const cst_string *string,
				const cst_string *whitespace,
				const cst_string *singlechars,
				const cst_string *prepunct,
				const cst_string *postpunct)
{
    cst_tokenstream *ts = new_tokenstream(whitespace,
					  singlechars,
					  prepunct,
					  postpunct);

    ts->string_buffer = cst_strdup(string);
    ts_getc(ts);

    return ts;
}

cst_tokenstream *ts_open_generic(const char *filename,
                                 const cst_string *whitespacesymbols,
                                 const cst_string *singlecharsymbols,
                                 const cst_string *prepunctsymbols,
                                 const cst_string *postpunctsymbols,
                                 void *streamtype_data,
                                 int (*open)(cst_tokenstream *ts,
                                             const char *filename),
                                 void (*close)(cst_tokenstream *ts),
                                 int (*eof)(cst_tokenstream *ts),
                                 int (*seek)(cst_tokenstream *ts, int pos),
                                 int (*tell)(cst_tokenstream *ts),
                                 int (*size)(cst_tokenstream *ts),
                                 int (*getc)(cst_tokenstream *ts))
{   /* Its a generic token stream where user has specified the low level */
    /* file/stream access functions                                      */
    cst_tokenstream *ts = new_tokenstream(whitespacesymbols,
					  singlecharsymbols,
					  prepunctsymbols,
					  postpunctsymbols);

    ts->streamtype_data = streamtype_data;
    ts->open = open;
    ts->close = close;
    ts->eof = eof;
    ts->seek = seek;
    ts->tell = tell;
    ts->size = size;
    ts->getc = getc;

    if ((ts->open)(ts,filename) != 0)
    {
        (ts->getc)(ts);
        return ts;
    }
    else
    {
	delete_tokenstream(ts);
	return NULL;
    }
}

void ts_close(cst_tokenstream *ts)
{
    if (ts->fd != NULL)
    {
#ifndef UNDER_CE
	if (ts->fd != stdin)
#endif
	    cst_fclose(ts->fd);
	ts->fd = NULL; /* just in case close gets called twice */
    }
    if (ts->string_buffer != NULL)
    {
        cst_free(ts->string_buffer);
	ts->string_buffer = NULL;
    }
    if (ts->open)
        (ts->close)(ts);
    delete_tokenstream(ts);
}

static void get_token_sub_part(cst_tokenstream *ts,
			       int charclass,
			       cst_string **buffer,
			       int *buffer_max)
{
    int p;

    for (p=0; ((!ts_eof(ts)) &&
               (ts_charclass(ts->current_char,charclass,ts)) &&
	       (!ts_charclass(ts->current_char,
			      TS_CHARCLASS_SINGLECHAR,ts))); p++)
    {
	if (p+1 >= *buffer_max) extend_buffer(buffer,buffer_max);
	(*buffer)[p] = ts->current_char;
	ts_getc(ts);
    }
    (*buffer)[p] = '\0';
}

 #if defined _WIN32 && !defined(__GNUC__)
 __inline int ts_utf8_sequence_length(char c0)
 #else
 int ts_utf8_sequence_length(char c0)
 #endif
 {
    /* Get the expected length of UTF8 sequence given its most */
    /* significant byte */
     return (( 0xE5000000 >> (( c0 >> 3 ) & 0x1E )) & 3 ) + 1;
 }

/* Can't afford dynamically generate this char class so have separated func */
/* so do the core token part -- this goes while not givenlass (while the    */
/* above function oes while is givenclass */
static void get_token_sub_part_2(cst_tokenstream *ts,
				 int endclass1,
				 cst_string **buffer,
				 int *buffer_max)
{
    int p;

    for (p=0; ((!ts_eof(ts)) &&
               (!ts_charclass(ts->current_char,endclass1,ts)) &&
	       (!ts_charclass(ts->current_char,
			      TS_CHARCLASS_SINGLECHAR,ts)));
         p++)
    {
	if (p+1 >= *buffer_max) extend_buffer(buffer,buffer_max);
	(*buffer)[p] = ts->current_char;
        /* If someone sets tags we end the token */
        /* This can't happen in standard tokenstreams, but can in user */
        /* defined ones */
        if (ts->tags) break;  

        /* In the special utf8 char by char mode we end at end of a utf8 char */
        if ((ts->utf8_explode_mode) &&
            (p == ts_utf8_sequence_length((*buffer)[0])))
            break;

	ts_getc(ts);
    }
    (*buffer)[p] = '\0';
}

static void get_token_postpunctuation(cst_tokenstream *ts)
{
    int p,t;

    t = cst_strlen(ts->token);
    for (p=t;
	 (p > 0) && 
	     ((ts->token[p] == '\0') ||
	      (ts_charclass(ts->token[p],TS_CHARCLASS_POSTPUNCT,ts)));
	 p--);

    if (t != p)
    {
	if (t-p >= ts->postp_max) 
	    extend_buffer(&ts->postpunctuation,&ts->postp_max);
	/* Copy postpunctuation from token */
	memmove(ts->postpunctuation,&ts->token[p+1],(t-p));
	/* truncate token at postpunctuation */
	ts->token[p+1] = '\0';
    }
}

int ts_eof(cst_tokenstream *ts)
{
    if (ts->eof_flag)
	return TRUE;
    else
	return FALSE;
}

int ts_set_stream_pos(cst_tokenstream *ts, int pos)
{
    /* Note this doesn't preserve line_pos */
    int new_pos, l;

    if (ts->fd)
    {
        new_pos = (int)cst_fseek(ts->fd,(long)pos,CST_SEEK_ABSOLUTE);
        if (new_pos == pos)
            ts->eof_flag = FALSE;
    }
    else if (ts->string_buffer)
    {
        l = cst_strlen(ts->string_buffer);
        if (pos > l)
            new_pos = l;
        else if (pos < 0)
            new_pos = 0;
        else
            new_pos = pos;
        ts->eof_flag = FALSE;
    }
    else if (ts->open)
    {
        new_pos = (ts->seek)(ts,pos);
        if (new_pos == pos)
            ts->eof_flag = FALSE;
    }
    else
        new_pos = pos;  /* not sure it can get here */
    ts->file_pos = new_pos;
    ts->current_char = ' ';  /* To be safe (but this is wrong) */

    return ts->file_pos;
}

int ts_get_stream_pos(cst_tokenstream *ts)
{
    if (ts->open)
        return (ts->tell)(ts);
    else
        return ts->file_pos;
}

int ts_get_stream_size(cst_tokenstream *ts)
{
    int current_pos, end_pos;
    if (ts->fd)
    {
        current_pos = ts->file_pos;
        end_pos = (int)cst_fseek(ts->fd,(long)0,CST_SEEK_ENDREL);
        cst_fseek(ts->fd,(long)current_pos,CST_SEEK_ABSOLUTE);
        return end_pos;
    } else if (ts->string_buffer)
        return cst_strlen(ts->string_buffer);
    else if (ts->open)
        return (ts->size)(ts);
    else
        return 0;
}

cst_string private_ts_getc(cst_tokenstream *ts)
{
    return internal_ts_getc(ts);
}

static cst_string ts_getc(cst_tokenstream *ts)
{
    if (ts->open)
        ts->current_char = (ts->getc)(ts);
    else
        ts->current_char = internal_ts_getc(ts);
    return ts->current_char;
}

static cst_string internal_ts_getc(cst_tokenstream *ts)
{
    if (ts->fd)
    {
	ts->current_char = cst_fgetc(ts->fd);
        if (ts->current_char == -1)
	    ts->eof_flag = TRUE;
    }
    else if (ts->string_buffer)
    {
	if (ts->string_buffer[ts->file_pos] == '\0')
        {
	    ts->eof_flag = TRUE;
	    ts->current_char = '\0';
        }
	else
	    ts->current_char = ts->string_buffer[ts->file_pos];
    }
    
    if (!ts_eof(ts))
	ts->file_pos++;
    if (ts->current_char == '\n')
	ts->line_number++;
    return ts->current_char;
}

const cst_string *ts_get_quoted_token(cst_tokenstream *ts,
					 char quote,
					 char escape)
{
    /* for reading the next quoted token that starts with quote and
       ends with quote, quote may appear only if preceded by escape */
    int p;

    /* Hmm can't change quotes within a ts */
    ts->charclass[(unsigned int)quote] |= TS_CHARCLASS_QUOTE;
    ts->charclass[(unsigned int)escape] |= TS_CHARCLASS_QUOTE;

    /* skipping whitespace */
    get_token_sub_part(ts,TS_CHARCLASS_WHITESPACE,
		       &ts->whitespace,
		       &ts->ws_max);
    ts->token_pos = ts->file_pos - 1;

    if (ts->current_char == quote)
    {   /* go until quote */
	ts_getc(ts);
        for (p=0; ((!ts_eof(ts)) &&
                   (ts->current_char != quote));
             p++)
        {
            if (p >= ts->token_max) 
                extend_buffer(&ts->token,&ts->token_max);
            ts->token[p] = ts->current_char;
            ts_getc(ts);
            if (ts->current_char == escape)
            {
                ts_get(ts);
                if (p >= ts->token_max) 
                    extend_buffer(&ts->token,&ts->token_max);
                ts->token[p] = ts->current_char;
                ts_get(ts);
            }
        }
        ts->token[p] = '\0';
	ts_getc(ts);
    }
    else /* its not quoted, like to be careful dont you */
    {    /* treat is as standard token                  */
	/* Get prepunctuation */
        extend_buffer(&ts->prepunctuation,&ts->prep_max);
	get_token_sub_part(ts,TS_CHARCLASS_PREPUNCT,
			   &ts->prepunctuation,
			   &ts->prep_max);
	/* Get the symbol itself */
	if (ts_charclass(ts->current_char,TS_CHARCLASS_SINGLECHAR,ts))
	{
	    if (2 >= ts->token_max) extend_buffer(&ts->token,&ts->token_max);
	    ts->token[0] = ts->current_char;
	    ts->token[1] = '\0';
	    ts_getc(ts);
	}
	else
	    get_token_sub_part_2(ts,
				 TS_CHARCLASS_WHITESPACE,    /* end class1 */
				 &ts->token,
				 &ts->token_max);
	/* This'll have token *plus* post punctuation in ts->token */
	/* Get postpunctuation */
	get_token_postpunctuation(ts);
    }

    return ts->token;
}

const cst_string *ts_get(cst_tokenstream *ts)
{
    /* Get next token */

    if (ts->tags)
    {  /* Someone didn't delete them before -- so we delete them now */
        delete_features(ts->tags);
        ts->tags = NULL;
    }

    /* Skip whitespace */
    get_token_sub_part(ts,
		       TS_CHARCLASS_WHITESPACE,
		       &ts->whitespace,
		       &ts->ws_max);

    /* quoted strings currently ignored */
    ts->token_pos = ts->file_pos - 1;
	
    /* Get prepunctuation */
    if (!ts_eof(ts) &&
        ts_charclass(ts->current_char,TS_CHARCLASS_PREPUNCT,ts))
	get_token_sub_part(ts,
			   TS_CHARCLASS_PREPUNCT,
			   &ts->prepunctuation,
			   &ts->prep_max);
    else if (ts->prepunctuation)
	ts->prepunctuation[0] = '\0';
    /* Get the symbol itself */
    if (!ts_eof(ts) &&
        ts_charclass(ts->current_char,TS_CHARCLASS_SINGLECHAR,ts))
    {
	if (2 >= ts->token_max) extend_buffer(&ts->token,&ts->token_max);
	ts->token[0] = ts->current_char;
	ts->token[1] = '\0';
	ts_getc(ts);
    }
    else
	get_token_sub_part_2(ts,
			     TS_CHARCLASS_WHITESPACE,       /* end class1 */
			     &ts->token,
			     &ts->token_max);
    /* This'll have token *plus* post punctuation in ts->token */
    /* Get postpunctuation */
    if (ts->postpunctuation)
	ts->postpunctuation[0] = '\0';
    if (ts->p_postpunctuationsymbols[0])
        get_token_postpunctuation(ts);

    return ts->token;
}

int ts_read(void *buff, int size, int num, cst_tokenstream *ts)
{
    /* people should complain about the speed here */
    /* people will complain about EOF as end of file */
    int i,j,p;
    cst_string *cbuff;

    cbuff = (cst_string *)buff;

    for (p=i=0; i < num; i++)
	for (j=0; j < size; j++,p++)
	    cbuff[p] = ts_getc(ts);

    return i;
}
