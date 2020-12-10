
         Flite: a small run-time speech synthesis engine
                      version 2.1-release
          Copyright Carnegie Mellon University 1999-2018
                      All rights reserved
                      http://cmuflite.org
              https://github.com/festvox/flite


Flite is an open source small fast run-time text to speech engine.  It
is the latest addition to the suite of free software synthesis tools
including University of Edinburgh's Festival Speech Synthesis System
and Carnegie Mellon University's FestVox project, tools, scripts and
documentation for building synthetic voices.  However, flite itself
does not require either of these systems to compile and run.

The core Flite library was developed by Alan W Black <awb@cs.cmu.edu>
(mostly in his so-called spare time) while employed in the Language
Technologies Institute at Carnegie Mellon University.  The name
"flite", originally chosen to mean "festival-lite" is perhaps doubly
appropriate as a substantial part of design and coding was done over
30,000ft while awb was travelling, and (usually) isn't in meetings.

The voices, lexicon and language components of flite, both their
compression techniques and their actual contents were developed by
Kevin A. Lenzo <lenzo@cs.cmu.edu> and Alan W Black <awb@cs.cmu.edu>.

Flite is the answer to the complaint that Festival is too big, too slow,
and not portable enough.

o Flite is designed for very small devices, such as PDAs, and also
  for large server machines which need to serve lots of ports.
  
o Flite is not a replacement for Festival but an alternative run time
  engine for voices developed in the FestVox framework where size and
  speed is crucial.
  
o Flite is all in ANSI C, it contains no C++ or Scheme, thus requires
  more care in programming, and is harder to customize at run time.
  
o It is thread safe

o Voices, lexicons and language descriptions can be compiled 
  (mostly automatically for voices and lexicons) into C representations 
  from their FestVox formats
  
o All voices, lexicons and language model data are const and in the
  text segment (i.e. they may be put in ROM).  As they are linked in
  at compile time, there is virtually no startup delay.
  
o Although the synthesized output is not exactly the same as the same 
  voice in Festival they are effectively equivalent.  That is, flite 
  doesn't sound better or worse than the equivalent voice in festival,
  just faster, smaller and scalable.
  
o For standard diphone voices, maximum run time memory
  requirements are approximately less than twice the memory requirement 
  for the waveform generated.  For 32bit architectures
  this effectively means under 1M.
  
o The flite program supports, synthesis of individual strings or files
  (utterance by utterance) to direct audio devices or to waveform files.
  
o The flite library offers simple functions suitable for use in specific
  applications.
  
Flite is distributed with a single 8K diphone voice (derived from the
cmu_us_kal voice), a pruned lexicon (derived from
cmulex) and a set of models for US English.  Here are comparisons
with Festival using basically the same 8KHz diphone voice

                Flite    Festival
    core code    60K      2.6M
    USEnglish    100K     ??
    lexicon      600K     5M
    diphone      1.8M     2.1M
    runtime      <1M      16-20M
   

On a 500Mhz PIII, a timing test of the first two chapters of
"Alice in Wonderland" (doc/alice) was done.  This produces about
1300 seconds of speech.  With flite it takes 19.128 seconds (about
70.6 times faster than real time) with Festival it takes 97 seconds
(13.4 times faster than real time).  On the ipaq (with the 16KHz diphones)
flite synthesizes 9.79 time faster than real time.

Requirements:
-------------

    o A good C compiler, some of these files are quite large and some C
      compilers might choke on these, gcc is fine.  Sun CC 3.01 has been
      tested too.  Visual C++ 6.0 is known to fail on the large diphone
      database files.  We recommend you use GCC Windows Subsystem for Linux
      Cygwin or mingw32 instead.
  
    o GNU Make

    o An audio device isn't required as flite can write its output to 
      a waveform file. 

Supported platforms:
--------------------

We have successfully compiled and run on 

    o Various Intel Linux systems (and iPaq Linux), under various versions
      of GCC (2.7.2 to 6.x)
  
    o Mac OS X

    o Various Android devices

    o Various openwrt devices

    o FreeBSD 3.x and 4.x

    o Solaris 5.7, and Solaris 9

    o Windows 2000/XP and later under Cygwin 1.3.5 and later

    o Windows 10 with Windows Subsystem for Linux

    o Successfully compiles and runs under 64Bit Linux architectures

    o OSF1 V4.0 (gives an unimportant warning about sizes when compiled cst_val.c)

    o WASI has experimental support (see below for details)

Previously we supported PalmOS and Windows CE but these seem to be rare
nowadays so they are no longer actively supported.

Other similar platforms should just work, we have also cross compiled
on a Linux machine for StrongARM.  However note that new byte order
architectures may not work directly as there is some careful
byte order constraints in some structures.  These are portable but may
require reordering of some fields, contact us if you are moving to
a new architecture.

### Cross-compiling to WASI (experimental)
In order to successfully cross-compile to WASI, firstly head over to
[CraneStation/wasi-sdk](https://github.com/CraneStation/wasi-sdk)
and install the WASI toolchain.

Afterwards, you can cross-compile to WASI as follows:

```
./configure --host=wasm32-wasi \
CC=/path/to/wasi-sdk/bin/clang \
AR=/path/to/wasi-sdk/bin/llvm-ar \
RANLIB=/path/to/wasi-sdk/bin/llvm-ranlib
```

It is important to correctly specify `ar` and `ranlib` that is bundled
with the WASI `clang`. Otherwise, you will most likely experience missing
symbols during linking, plus you may experience weird `llvm` errors such as

```
LLVM ERROR: malformed uleb128, extends past end
```

When cross-compiling from macOS, you might have to manually specify the sysroot.
You can do this by tweaking the `CC` variable as follows:

```
CC="/path/to/wasi-sdk/bin/clang --sysroot=/path/to/wasi-sdk/share/sysroot"
```

After the configure step is successful, simply run as usual:
```
make
```

The generated WASI binary can then be found in `bin/` directory:

```
file bin/flite
> bin/flite: WebAssembly (wasm) binary module version 0x1 (MVP)
```

News
----

New in 2.2 (Oct 2018)
    o Better grapheme support (Wilderness Languages) hundreds of new languages
    
New in 2.1 (Oct 2017)

    o Improved Indic front end support (thanks to Suresh Bazaj @ Hear2Read)
    
    o 18 English Voices (various accents)
    
    o 12 Indian Voices (Bengali, Gujarati, Hindi, Kannada, Marathi, Panjabi
      Tamil and Telugu) usually with bilingual (with English) support
      
    o Can do byteswap architectures [again] (ar9331 yun arduino, zsun etc)
    
    o flitecheck front-end test suite
    
    o grapheme based festvox builds give working flitevox voices
    
    o SAPI support for CG voices (thanks to Alok Parlikar @ Cobalt Speech and
      Language INC)
      
    o gcc 6.x support
    
    o .flitevox files (and models) 40% of previous size, but same quality

New in 2.0.0 (Dec 2014)
    o Indic language support (Hindi, Tamil and Telugu)
    
    o SSML support
    
    o CG voices as files accessilble by file:/// and http://
      (and set of 13 voices to load)
      
    o random forest (multimodel support) improves voice quality
    
    o Supports diffrent sample rates/mgc order to tune for speed
    
    o Kal diphone 500K smaller
    
    o Fixed lots of API issues
    
    o thread safe (again) [after initialization]
    
    o Generalized tokenstreams (used in Bard Storyteller)
    
    o simple-Pulseaudio support
    
    o Improved Android support
    
    o Removed PalmOS support from distribution
    
    o Companion multilingual ebook reader Bard Storyteller 
       https://github.com/festvox/bard

New in 1.4.1 (March 2010)
    o better ssml support (actually does something)
    
    o better clunit support (smaller)
    
    o Android support

New in 1.4 (December 2009)
    o crude multi-voice selection support (may change)
    
    o 4 basic voices are included 3 clustergen (awb, rms and slt) plus
      the kal diphone database
      
    o CMULEX now uses maximum onset for syllabification
    
    o alsa support
    
    o Clustergen support (including mlpg with mixed excitation) 
      But is still slow on limited processors
      
    o Windows support with Visual Studio (specifically for the Olympus 
        Spoken Dialog System)
        
    o WinCE support is redone with cegcc/mingw32ce with example
        example TTS app: Flowm: Flite on Windows Mobile
        
    o Speed-ups in feature interpretation limiting calls to alloc
    
    o Speed-ups (and fixes) for converting clunits festvox voices

New in 1.3-release (October 2005)
    o fixes to lpc residual extraction to give better quality output
    
    o An updated lexicon (festlex_CMU from festival-2.0.95) and better
      compression its about 30% of the previous size, with about
      the same accuracy
    o Fairly substantial code movements to better support PalmOS and
      multi-platform cross compilation builds
      
    o A PalmOS 5.0 port with an small example talking app ("flop")
    
    o runs under ix86_64 linux

New in 1.2-release  (February 2003)
    o A build process for diphone and clunits/ldom voices
      FestVox voices can be converted (sometimes) automatically
      
    o Various bug fixes
    
    o Initial support for Mac OS X (not talking to audio device yet)
      but compiles and runs
      
    o Text files can be synthesize to a single audio file
    
    o (optional) shared library support (Linux)

Compilation
-----------

In general

    tar zxvf flite-2.1-current.tar.gz
    
    cd flite-2.1-current
    ./configure 
    make
    make get_voices

Where tar is gnu tar (gtar), and make is gnu make (gmake).

Or

    git clone http://github.com/festvox/flite
    cd flite
    ./configure
    make
    make get_voices

Configuration should be automatic, but maybe doesn't work in all cases
especially if you have some new compiler.  You can explicitly set the
compiler in config/config and add any options you see fit.  Configure
tries to guess these but it might be unable to guess for cross
compilation cases Interesting options there are

    -DWORDS_BIGENDIAN=1  for bigendian machines (e.g. Sparc, M68x, ar9331)
    -DNO_UNION_INITIALIZATION=1  For compilers without C 99 union inintialization
    -DCST_AUDIO_NONE     if you don't need/want audio support

There are different sets of voices and languages you can select between
them (and your own sets if you make config/XXX.lv).  For example

    ./configure --with-langvox=transtac

Will use the languages and voices defined in config/transtac.lv

Usage:
------

The ./bin/flite binary contains all supported voices and you may
choose between the voices with the -voice flag and list the supported
voices with the -lv flag.  Note the kal (diphone) voice is a different
technology from the others and is much less computationally expensive
but more robotic.  For each voice additional binaries that contain
only that voice are created in ./bin/flite_FULLVOICENAME,
e.g. ./bin/flite_cmu_us_awb.  You can also refer to external clustergen
.flitevox voice via a pathname argument with -voice (note the pathname
must contain at least one "/")

If it compiles properly a binary will be put in bin/, note by
default -g is on so it will be bigger than is actually required

    ./bin/flite "Flite is a small fast run-time synthesis engine" flite.wav

Will produce an 8KHz riff headered waveform file (riff is Microsoft's
wave format often called .WAV).

    ./bin/flite doc/alice

Will play the text file doc/alice.  If the first argument contains
a space it is treated as text otherwise it is treated as a filename.
If a second argument is given a waveform file is written to it,
if no argument is given or "play" is given it will attempt to 
write directly to the audio device (if supported).  if "none"
is given the audio is simply thrown away (used for benchmarking).
Explicit options are also available.

    ./bin/flite -v doc/alice none

Will synthesize the file without playing the audio and give a summary
of the speed.

    ./bin/flite doc/alice alice.wav

will synthesize the whole of alice into a single file (previoous
versions would only give the last utterance in the file, but
that is fixed now).

An additional set of feature setting options are available, these are
*debug* options, Voices are represented as sets of feature values (see
lang/cmu_us_kal/cmu_us_kal.c) and you can override values on the
command line.  This can stop flite from working if malicious values
are set and therefor this facility is not intended to be made
available for standard users.  But these are useful for
debugging.  Some typical examples are

Use simple concatenation of diphones without prosodic modification

    ./bin/flite --sets join_type=simple_join doc/intro

Print sentences as they are said

    ./bin/flite -pw doc/alice

Make it speak slower

    ./bin/flite --setf duration_stretch=1.5 doc/alice

Make it speak higher pitch

    ./bin/flite --setf int_f0_target_mean=145 doc/alice

The talking clock is an example talking clode as discussed on
http://festvox.org/ldom it requires a single argument HH:MM
under Unix you can call it

    ./bin/flite_time `date +%H:%M`

List the voices linked in directly in this build

    ./bin/flite -lv

Speak with the US male rms voice (builtin version)

    ./bin/flite -voice rms -f doc/alice

Speak with the "Scottish" male awb voice (builtin version)

    ./bin/flite -voice awb -f doc/alice

Speak with the US female slt voice

    ./bin/flite -voice slt -f doc/alice

Speak with AEW voice, download on the fly from festvox.org

    ./bin/flite -voice http://festvox.org/flite/packed/flite-2.1/voices/cmu_us_aew.flitevox -f doc/alice

Speak with AHW voice loaded from the local file.

    ./bin/flite -voice voices/cmu_us_ahw.flitevox -f doc/alice

You can download the available voices into voices/

    ./bin/get_voices us_voices

and/or

    ./bin/get_voices indic_voices

Voice quality
-------------

So you've eagerly downloaded flite, compiled it and run it, now you
are disappointed that it doesn't sound wonderful, sure its fast and
small but what you really hoped for was the dulcit tones of a deep
baritone voice that would make you desperately hang on every phrase it
mellifluously produces.  But instead you get an 8Khz diphone voice that
sounds like it came from the last millenium.

Well, first, you are right, it is an 8KHz diphone voice from the last
millenium, and that was actually deliberate.  As we developed flite we
wanted a voice that was stable and that we could directly compare with
that very same voice in Festival.  Flite is an *engine*.  We want to
be able take voices built with the FestVox process and compile them
for flite, the result should be exactly the same quality (though of
course trading the size for quality in flite is also an option).  The
included voice is just a sample voice that was used in the testing
process.  

We expect that often voices will be loaded from external files, and we
have now set up a voice repository in

    http://festvox.org/flite/flite-2.1/voices/*.flitevox
   
If you visit there with a browser you can hear the examples.  You can
also download the .flitevox files to you machine so you don't need a
network connect everytime you need to load a voice.

We are now actively adding to this list of available voices in English (16)
and other languages.

Bard Storyteller:  https://github.com/festvox/bard
--------------------------------------------------

Bard is a companion app that reads ebooks, both displaying them and
actually reading them to you out loud using flite.  Bard supports a
wide range of fonts, and flite voices, and books in text, html and
epub format.  Bard is used as a evaluation of flite's capabilities and
an example of a serious application using flite.

