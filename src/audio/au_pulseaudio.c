/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2012                            */
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
/*********************************************************************** */
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  February 2012                                    */
/*************************************************************************/
/*                                                                       */
/*  Native access to pulseaudio on Linux                                 */
/*  (surprisingly hard to find programming documentation about pa)       */
/*  Sort of got it from here                                             */
/*  http://pulseaudio.org/                                               */
/*                                                                       */
/*************************************************************************/

#include "cst_string.h"
#include "cst_wave.h"
#include "cst_audio.h"

#include <pulse/simple.h>

cst_audiodev *audio_open_pulseaudio(unsigned int sps, int channels, cst_audiofmt fmt)
{
  cst_audiodev *ad;
  int err=0;

  /* Pulseaudio specific stuff */
  pa_sample_spec *ss;
  pa_simple *s;

  ss = cst_alloc(pa_sample_spec,1);
  ss->rate = sps;
  ss->channels = channels;
  switch (fmt)
  {
  case CST_AUDIO_LINEAR16:
	if (CST_LITTLE_ENDIAN)
            ss->format = PA_SAMPLE_S16LE;
	else
            ss->format = PA_SAMPLE_S16BE;
	break;
  case CST_AUDIO_LINEAR8:
      ss->format = PA_SAMPLE_U8;
      break;
  case CST_AUDIO_MULAW:
      ss->format = PA_SAMPLE_ULAW;
      break;
  default:
      return NULL;
      break;
  }

  s = pa_simple_new(
                    NULL,      /* use default server */
                    "flite",
                    PA_STREAM_PLAYBACK,
                    NULL,      /* use default device */
                    "Speech",
                    ss,
                    NULL,      /* default channel map */
                    NULL,      /* default buffering attributes */
                    &err);
  if (err < 0)
      return NULL;

  /* Write hardware parameters to flite audio device data structure */
  ad = cst_alloc(cst_audiodev, 1);
  ad->real_sps = ad->sps = sps;
  ad->real_channels = ad->channels = channels;
  ad->real_fmt = ad->fmt = fmt;
  ad->platform_data = (void *) s;

  return ad;
}

int audio_close_pulseaudio(cst_audiodev *ad)
{
  int result;
  pa_simple *s;

  if (ad == NULL)
      return 0;

  s = (pa_simple *) ad->platform_data;

  pa_simple_drain(s,&result);

  pa_simple_free(s);
  cst_free(ad);
  return result;
}

int audio_write_pulseaudio(cst_audiodev *ad, void *samples, int num_bytes)
{
    pa_simple *s;
    int err;

    s = (pa_simple *)ad->platform_data;
    pa_simple_write(s,samples,(size_t)num_bytes,&err);

    return num_bytes;
}

int audio_flush_pulseaudio(cst_audiodev *ad)
{
    pa_simple *s;
    int err;

    s = (pa_simple *)ad->platform_data;
    pa_simple_drain(s,&err);
    
    return err;
}

int audio_drain_pulseaudio(cst_audiodev *ad)
{
    pa_simple *s;
    int err;

    s = (pa_simple *)ad->platform_data;
    pa_simple_drain(s,&err);
    
    return err;
}

