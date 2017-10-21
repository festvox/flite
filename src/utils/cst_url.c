/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2011                            */
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
/*               Date:  November 2011                                    */
/*************************************************************************/
/*                                                                       */
/*  Support to access (some forms of) url                                */
/*      http:  and file:                                                 */
/*  Should support libcurl if available -- but a simple form if not      */
/*                                                                       */
/*  Only support http: if sockets are available                          */
/*                                                                       */
/*************************************************************************/
#include "cst_math.h"
#include "cst_file.h"
#include "cst_string.h"
#include "cst_tokenstream.h"
#include "cst_socket.h"

#ifndef CST_NO_SOCKETS
#ifndef _MSC_VER
#include <stdlib.h>
#include <unistd.h>
#else
#include <io.h>
#include <WinSock.h>
#endif
#endif

int cst_urlp(const char *url)
{
    /* Return 1 if url is a url, 0 otherwise */
    /* This is decided by the initial substring being "http:" or "file:" */
    if ((cst_strlen(url) > 4) &&
        (cst_streqn("http:",url,5) ||
         cst_streqn("file:",url,5)))
        return TRUE;
    else
        return FALSE;
}

cst_file cst_url_open(const char *url)
{
    /* Always opens it for reading */
    cst_tokenstream *urlts;
    const cst_string *protocol;
    int port;
    cst_string *host;
    int fd;
    char *url_request;
    char *path;
    cst_file ofd;
    int state,n;
    char c;

    urlts = ts_open_string(url, "", ":/", "", "");

    protocol = ts_get(urlts);
    if (cst_streq(protocol,"http"))
    {
#ifdef CST_NO_SOCKETS
        ts_close(urlts);
        return NULL;
#else
        if (!cst_streq(ts_get(urlts),":") ||
            !cst_streq(ts_get(urlts),"/") ||
            !cst_streq(ts_get(urlts),"/"))
        {
            ts_close(urlts);
            return NULL;
        }
        host = cst_strdup(ts_get(urlts));
        if (cst_streq(ts_get(urlts),":"))
            port = (int)cst_atof(ts_get(urlts));
        else
            port = 80;

        /* Open port to web server */
        fd = cst_socket_open(host,port);
        if (fd < 0)
        {
            cst_free(host);
            ts_close(urlts);
            return NULL;
        }

        url_request = cst_alloc(char,cst_strlen(url)+17);
        cst_sprintf(url_request,"GET %s HTTP/1.2\n\n",url);
        n = write(fd,url_request,cst_strlen(url_request));
        cst_free(url_request);

        /* Skip http header -- until \n\n */
        state=0;
        while (state != 4)
        {
            n=read(fd,&c,1);
            if (n == 0)
            {   /* eof or link gone down */
                cst_free(host);
                ts_close(urlts);
                return NULL;
            }
            if ((state == 0) && (c == '\r'))
                state=1;
            else if ((state == 1) && (c == '\n'))
                state=2;
            else if ((state == 2) && (c == '\r'))
                state=3;
            else if ((state == 3) && (c == '\n'))
                state=4;
            /* Not sure you can get no CRs in the stream */
            else if ((state == 0) && (c == '\n'))
                state=2;
            else if ((state == 2) && (c == '\n'))
                state=4;
            else
                state = 0;
        }

        ofd = fdopen(fd,"rb");

        ts_close(urlts);
        cst_free(host);

        return ofd;
#endif
    }
    else if (cst_streq(protocol,"file"))
    {
        if (!cst_streq(ts_get(urlts),":") ||
            !cst_streq(ts_get(urlts),"/") ||
            !cst_streq(ts_get(urlts),"/"))
        {
            ts_close(urlts);
            return NULL;
        }
        path = cst_strdup(&urlts->string_buffer[urlts->file_pos-1]);
        /* printf("awb_debug fileurl %s\n",path); */

        ofd = cst_fopen(path,CST_OPEN_READ);

        ts_close(urlts);
        cst_free(path);
        
        return ofd;
    }
    else
    {   /* Unsupported protocol */
        return NULL;
    }
}

