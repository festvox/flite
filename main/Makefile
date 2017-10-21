###########################################################################
##                                                                       ##
##                  Language Technologies Institute                      ##
##                     Carnegie Mellon University                        ##
##                       Copyright (c) 2001-2008                         ##
##                        All Rights Reserved.                           ##
##                                                                       ##
##  Permission is hereby granted, free of charge, to use and distribute  ##
##  this software and its documentation without restriction, including   ##
##  without limitation the rights to use, copy, modify, merge, publish,  ##
##  distribute, sublicense, and/or sell copies of this work, and to      ##
##  permit persons to whom this work is furnished to do so, subject to   ##
##  the following conditions:                                            ##
##   1. The code must retain the above copyright notice, this list of    ##
##      conditions and the following disclaimer.                         ##
##   2. Any modifications must be clearly marked as such.                ##
##   3. Original authors' names are not deleted.                         ##
##   4. The authors' names are not used to endorse or promote products   ##
##      derived from this software without specific prior written        ##
##      permission.                                                      ##
##                                                                       ##
##  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         ##
##  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      ##
##  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   ##
##  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      ##
##  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    ##
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   ##
##  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          ##
##  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       ##
##  THIS SOFTWARE.                                                       ##
##                                                                       ##
###########################################################################
##                                                                       ##
##  Top level user programs                                              ##
##                                                                       ##
###########################################################################
TOP=..
DIRNAME=main
BUILD_DIRS = 
ALL_DIRS= 
SRCS = flite_main.c flite_time_main.c t2p_main.c compile_regexes.c \
       flitevox_info_main.c word_times_main.c
#      world_main.c 
OBJS = $(SRCS:.c=.o) flite_voice_list.o 
FILES = Makefile $(SRCS)
LOCAL_INCLUDES = 

ALL = shared_libs \
      $(BINDIR)/flite$(EXEEXT) \
      $(BINDIR)/t2p$(EXEEXT) $(BINDIR)/compile_regexes$(EXEEXT) \
      $(BINDIR)/flitevox_info$(EXEEXT) \
      flite_voice_list.c each $(EXTRABINS)

VOICES=$(VOXES)
VOICELIBS=$(VOICES:%=flite_%)

flite_LIBS = $(VOICELIBS) $(LANGS:%=flite_%) $(LEXES:%=flite_%) flite

flite_LIBS_flags = -L$(LIBDIR) $(flite_LIBS:%=-l%)
flite_LIBS_deps = $(flite_LIBS:%=$(LIBDIR)/lib%.a)

include $(TOP)/config/common_make_rules

# We need to set these vars, even if we're not going to build them
# so make clean can remove them
SHAREDARLIBS= $(flite_LIBS:%=$(LIBDIR)/lib%.shared.a)
SHAREDLIBS = $(SHAREDARLIBS:%.shared.a=%.so)
VERSIONSHAREDLIBS = $(SHAREDLIBS:%=%.${PROJECT_VERSION}) \
                    $(SHAREDLIBS:%=%.${PROJECT_SHLIB_VERSION})

flite_time_LIBS = flite_cmu_time_awb flite_usenglish flite_cmulex
flite_time_LIBS_flags = -L$(LIBDIR) $(flite_time_LIBS:%=-l%)
flite_time_LIBS_deps = $(flite_time_LIBS:%=$(LIBDIR)/lib%.a)

LOCAL_CLEAN = $(BINDIR)/flite$(EXEEXT) $(BINDIR)/flite_time$(EXEEXT) \
              $(BINDIR)/t2p$(EXEEXT) \
              $(SHAREDARLIBS) $(SHAREDLIBS) $(VERSIONSHAREDLIBS) \
              $(flite_LIBS_deps) $(VOICES:%=$(BINDIR)/flite_%) \
              flite_voice_list.c

ifdef SHFLAGS
flite_LIBS_flags += -Wl,-rpath $(LIBDIR) 
shared_libs: $(SHAREDLIBS)
else
shared_libs: nothing
endif

.NOTPARALLEL: $(ALL)

flite_lang_list: 
	rm -f flite_lang_list.c
	$(TOP)/tools/make_lang_list $(LANGS) $(LEXES)
	$(MAKE) flite_lang_list.o

$(BINDIR)/flite$(EXEEXT): flite_main.o flite_lang_list $(flite_LIBS_deps)
	$(TOP)/tools/make_voice_list $(VOICES)
	rm -f flite_voice_list.o
	$(MAKE) flite_voice_list.o
	$(CC) $(CFLAGS) -o $@ flite_main.o flite_voice_list.o flite_lang_list.o $(flite_LIBS_flags) $(LDFLAGS)

$(BINDIR)/flitevox_info$(EXEEXT): flitevox_info_main.o flite_lang_list $(flite_LIBS_deps)
	$(TOP)/tools/make_voice_list $(VOICES)
	rm -f flite_voice_list.o
	$(MAKE) flite_voice_list.o
	$(CC) $(CFLAGS) -o $@ flitevox_info_main.o flite_voice_list.o flite_lang_list.o $(flite_LIBS_flags) $(LDFLAGS)

$(BINDIR)/world$(EXEEXT): world_main.c
	$(CC) $(CFLAGS) -o $@ world_main.c -I../include -I../src/world $(flite_LIBS_flags) $(world_LIBS_flags) -lworld $(LDFLAGS)

$(BINDIR)/flite_time$(EXEEXT): flite_time_main.o $(flite_time_LIBS_deps)
	$(CC) $(CFLAGS) -o $@ flite_time_main.o $(flite_time_LIBS_flags) $(FLITELIBFLAGS) $(LDFLAGS) 

$(BINDIR)/t2p$(EXEEXT): t2p_main.o $(flite_LIBS_deps)
	$(CC) $(CFLAGS) -o $@ t2p_main.o $(flite_LIBS_flags) $(LDFLAGS)

$(BINDIR)/word_times$(EXEEXT): word_times_main.o $(flite_LIBS_deps)
	$(CC) $(CFLAGS) -o $@ word_times_main.o $(flite_LIBS_flags) $(LDFLAGS)

$(BINDIR)/compile_regexes$(EXEEXT): compile_regexes.o $(FLITELIBS)
	$(CC) $(CFLAGS) -o $@ compile_regexes.o $(FLITELIBFLAGS) $(LDFLAGS)

each:
	@ for i in $(VOICES) ; \
	do \
	   $(MAKE) VOICE=$$i $(BINDIR)/flite_$$i ; \
	done

$(BINDIR)/flite_${VOICE}: flite_main.o flite_lang_list $(flite_LIBS_deps)
	$(TOP)/tools/make_voice_list $(VOICE)
	rm -f flite_voice_list.o
	$(MAKE) flite_voice_list.o
	$(CC) $(CFLAGS) -o $@ flite_main.o flite_voice_list.o flite_lang_list.o $(flite_LIBS_flags) $(LDFLAGS)

install:
#       The basic binaries
	@ $(INSTALL) -m 755 $(BINDIR)/flite $(DESTDIR)$(INSTALLBINDIR)
	@ for i in $(VOICES) ; \
	do \
	   $(INSTALL) $(BINDIR)/flite_$$i $(DESTDIR)$(INSTALLBINDIR); \
	done
	$(INSTALL) -m 755 $(BINDIR)/flite_time $(DESTDIR)$(INSTALLBINDIR)
#       The libraries: static and shared (if built)
	cp -pd $(flite_LIBS_deps) $(DESTDIR)$(INSTALLLIBDIR)
ifdef SHFLAGS
	cp -pd $(SHAREDLIBS) $(VERSIONSHAREDLIBS) $(DESTDIR)$(INSTALLLIBDIR)
endif

