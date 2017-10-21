/*************************************************************************/
/*                  Cobalt Speech and Language, Inc                      */
/*                         Copyright (c) 2015                            */
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
/*  COBALT SPEECH AND LANGUAGE INC AND THE CONTRIBUTORS TO THIS WORK     */
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
/*             Authors:  Alok Parlikar (alok@cobaltspeech.com)           */
/*                Date:  December 2015                                   */
/*                                                                       */
/*           Thanks to VocaliD Inc. for supporting this work.            */
/*************************************************************************/
// FliteCMUGenericCGObj.h : Declaration of the CFliteCMUGenericCGObj

#pragma once
#include "resource.h"       // main symbols



#include "FliteCMUGenericCG_i.h"
#include "..\FliteTTSEngineObj\FliteTTSEngineObj.h"
#include "flite_sapi_usenglish.h"

//#define VOXNAME cmu_us_awb
//#define REGISTER_VOX register_cmu_us_awb
//#define UNREGISTER_VOX unregister_cmu_us_awb
//#define VOXHUMAN "awb"
//#define VOXGENDER "unknown"
//#define VOXVERSION 1.0


extern "C" {
	cst_voice *REGISTER_VOX(const char *voxdir);
	void UNREGISTER_VOX(cst_voice *vox);

	void usenglish_init(cst_voice *v);
	cst_lexicon *cmulex_init(void);
 
        // Sai Krishna + Tim 13 March 2017

	void cmu_indic_lang_init(cst_voice *v);
	cst_lexicon *cmu_indic_lex_init(void);

	void cmu_grapheme_lang_init(cst_voice *v);
	cst_lexicon *cmu_grapheme_lex_init(void);
 
        /////////////////////////////////
};

class ATL_NO_VTABLE CFliteCMUGenericCGObj:
	public CComCoClass<CFliteCMUGenericCGObj, &CLSID_FliteCMUGenericCGObj>,
	public CFliteTTSEngineObj
{
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_FLITECMUGENERICCGOBJ)

	BEGIN_COM_MAP(CFliteCMUGenericCGObj)
		COM_INTERFACE_ENTRY(ISpTTSEngine)
		COM_INTERFACE_ENTRY(ISpObjectWithToken)
	END_COM_MAP()

public:
	CFliteCMUGenericCGObj() {
		regfunc = REGISTER_VOX;
		unregfunc = UNREGISTER_VOX;
		phonemefunc = flite_sapi_usenglish_phoneme;
		visemefunc = flite_sapi_usenglish_viseme;
		featurefunc = flite_sapi_usenglish_feature;
		pronouncefunc = flite_sapi_usenglish_pronounce;
	}
	~CFliteCMUGenericCGObj() {}
};


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
