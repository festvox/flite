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
/*             Authors:  Gopala Anumanchipalli (gopalakr@cs.cmu.edu)     */
/*                Date:  November 2011                                   */
/*************************************************************************/

#include "cst_cg.h"
#include "cst_hrg.h"
#include "cst_utt_utils.h"
#include "cst_spamf0.h"
#include "cst_math.h"

static void cst_synthtilt(const cst_cg_db *cg_db, 
                          const float start, const float peak, const float tiltamp, 
                          const float tiltdur, const float tilttilt,
                          cst_track *ftrack);

cst_utterance *cst_spamf0(cst_utterance *utt)
{
    cst_track *spamf0_track=NULL;
    cst_track *param_track=NULL;
    cst_item *s;
    cst_cg_db *cg_db;
    const cst_cart *acc_tree, *phrase_tree;
    float end,f0val, syldur;
    int num_frames,f,i;
    cg_db = val_cg_db(utt_feat_val(utt,"cg_db"));

    spamf0_track=new_track();
    cst_track_resize(spamf0_track,
                     (utt_feat_int(utt,"param_track_num_frames")),
                     1);
    acc_tree = cg_db->spamf0_accent_tree;
    phrase_tree = cg_db->spamf0_phrase_tree;
    end = 0.0;
    num_frames = 0;
    for (s = utt_rel_head(utt,"Segment"); s; s=item_next(s))
    {
        end = ffeature_float(s,"end");
        if(cst_streq("pau",ffeature_string(s,"name")))
        {
            f0val=0;
        }
        else
        {
            f0val=val_float(cart_interpret(s,phrase_tree));
        }

        for ( ; ((num_frames * cg_db->frame_advance) <= end) && (num_frames < utt_feat_int(utt,"param_track_num_frames")); num_frames++)
        {
            spamf0_track->frames[num_frames][0]=f0val;
        }
    }

    for (s=utt_rel_head(utt,"Syllable"); s; s=item_next(s))
    {
        f = val_int(cart_interpret(s,acc_tree));
        syldur = ffeature_float(s,"R:SylStructure.daughtern.R:Segment.end")
            - ffeature_float(s,"R:SylStructure.daughter1.R:Segment.p.end");
        cst_synthtilt(cg_db,
		      ffeature_float(s,"R:SylStructure.daughter1.R:Segment.p.end"),
                      cg_db->spamf0_accent_vectors[f][0],
                      cg_db->spamf0_accent_vectors[f][2],
                      syldur,
                      cg_db->spamf0_accent_vectors[f][6],
                      spamf0_track);
    }
    param_track = val_track(utt_feat_val(utt,"param_track"));
    for (i=0;i<utt_feat_int(utt,"param_track_num_frames");i++)
    {
        param_track->frames[i][0]=spamf0_track->frames[i][0];
    }
    delete_track(spamf0_track);
    return utt;
}

void cst_synthtilt(const cst_cg_db *cg_db, 
                          const float start, const float peak, const float tiltamp, 
                          const float tiltdur, const float tilttilt,
                          cst_track *ftrack)
{
    float arise,afall,drise,dfall,i;
    int num_frames;
    arise= tiltamp*(1+tilttilt)/2;
    afall= tiltamp*(1-tilttilt)/2;
    drise= tiltdur*(1+tilttilt)/2;
    dfall= tiltdur*(1-tilttilt)/2;
    num_frames=(int)ceil((double)(start/cg_db->frame_advance));
    /* Synthesizing the rise event */
    for (i=cg_db->frame_advance;((num_frames * cg_db->frame_advance)<(start+(drise/2))) ; num_frames++,i+=cg_db->frame_advance)
    {
        ftrack->frames[num_frames][0]+= peak - arise + (2 * arise * (i/drise) * (i/drise));
        ftrack->frames[num_frames][0]=exp(ftrack->frames[num_frames][0]);
    }
    for (;((num_frames * cg_db->frame_advance)<(start+drise)) ; num_frames++,i+=cg_db->frame_advance)
    {
        ftrack->frames[num_frames][0]+= peak - 2 * arise * (1- (i/drise)) * (1- (i/drise));
        ftrack->frames[num_frames][0]=exp(ftrack->frames[num_frames][0]);
    }
    /* Synthesizing the fall event */
    for (i=cg_db->frame_advance;((num_frames * cg_db->frame_advance)<(start+drise+(dfall/2))) ; num_frames++,i+=cg_db->frame_advance)
    {
        ftrack->frames[num_frames][0]+= peak + afall - (2 * afall * (i/dfall) * (i/dfall)) - afall;
        ftrack->frames[num_frames][0]=exp(ftrack->frames[num_frames][0]);
    }
    for (;((num_frames * cg_db->frame_advance)<(start+drise+dfall)) ; num_frames++,i+=cg_db->frame_advance)
    {
        ftrack->frames[num_frames][0]+= peak + (2 * afall * (1- (i/dfall)) * (1- (i/dfall))) - afall;
        ftrack->frames[num_frames][0]=exp(ftrack->frames[num_frames][0]);
    }
    return ;
}
