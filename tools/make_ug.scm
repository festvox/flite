;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;                                                                     ;;;
;;;                  Language Technologies Institute                    ;;;
;;;                     Carnegie Mellon University                      ;;;
;;;                         Copyright (c) 2013                          ;;;
;;;                        All Rights Reserved.                         ;;;
;;;                                                                     ;;;
;;; Permission is hereby granted, free of charge, to use and distribute ;;;
;;; this software and its documentation without restriction, including  ;;;
;;; without limitation the rights to use, copy, modify, merge, publish, ;;;
;;; distribute, sublicense, and/or sell copies of this work, and to     ;;;
;;; permit persons to whom this work is furnished to do so, subject to  ;;;
;;; the following conditions:                                           ;;;
;;;  1. The code must retain the above copyright notice, this list of   ;;;
;;;     conditions and the following disclaimer.                        ;;;
;;;  2. Any modifications must be clearly marked as such.               ;;;
;;;  3. Original authors' names are not deleted.                        ;;;
;;;  4. The authors' names are not used to endorse or promote products  ;;;
;;;     derived from this software without specific prior written       ;;;
;;;     permission.                                                     ;;;
;;;                                                                     ;;;
;;; CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK        ;;;
;;; DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING     ;;;
;;; ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT  ;;;
;;; SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE     ;;;
;;; FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   ;;;
;;; WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN  ;;;
;;; AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,         ;;;
;;; ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF      ;;;
;;; THIS SOFTWARE.                                                      ;;;
;;;                                                                     ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;             Author: Alan W Black (awb@cs.cmu.edu)                   ;;;
;;;               Date: May 2013                                        ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;                                                                     ;;;
;;; Various functions for universal grapheme voice support              ;;;
;;;                                                                     ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define (xtol name alist ofd)
  "((xtol name alist ofd)
Assoc list to ordered struction mapping a symbol to n other symbols."
  (format ofd "const int num_%s = %d;\n" name (length alist))
  (format ofd "const char * const %s[%d][5] =\n{\n" name (+ 1 (length alist)))
  (mapcar
   (lambda (x)
     (format t ">%l<\n" (car x))
     (format ofd "   {\"%s\", " (car x))
     (set! xx (car (cadr x)))
     (if (consp xx) 
         (format ofd "\"%l\", " (car xx))
         (format ofd "NULL, "))
     (if (consp (cdr xx)) 
         (format ofd "\"%l\", " (cadr xx))
         (format ofd "NULL, "))
     (if (consp (cddr xx)) 
         (format ofd "\"%l\", " (cadr (cdr xx)))
         (format ofd "NULL, "))
     (format ofd "NULL},\n")
     )
   alist)

  (format ofd "   { NULL, NULL, NULL, NULL }\n};\n\n")
)

(define (doit)

  (set! ofd (fopen "grapheme_unitran_tables.c" "w"))
  (format ofd "/* Copyright 2008-2012, University of Illinois at Urbana-Champaign          */\n")
  (format ofd "/* distributed under the Apache License, Version (2.0)                      */\n")
  (format ofd "/* http://www.apache.org/licenses/LICENSE-2.0                               */\n")
  (format ofd "/* Original table developed by Richard Sproat and Kyoung-young Kim          */\n")
  (format ofd "/* Ported for Festvox by Gopala Anumachipalli gopalakr@cs.cmu.edu Sep 2012  */\n")
  (format ofd "/* Then converted to C for CMU Flite (cmuflite.org)                         */\n")
  (format ofd "#include <flite.h>\n")
 
  (xtol "unicode_sampa_mapping" unicode_sampa_mapping ofd)

;  (xtol "grapheme_unicode_mapping" grapheme_unicode_mapping ofd)

  (fclose ofd))

(provide 'ug)
