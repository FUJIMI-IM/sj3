/*
 * Copyright (c) 1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: eucmessage.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1994/12/09 11:27:08 $
 */



#include "wchar16.h"
#include <locale.h>
#include "kctype.h"
#include "sj3libsj3.h"
#include "sj2.h"

unsigned char *EucMessages[] = {
 (u_char *)"\243\323\243\312\243\263\241\241\241\241\241\241\241\241\241\241\241\241\241\241",

 (u_char *)"Buffer\241\241\241\241",
 (u_char *)"Unbuffer\241\241",

 (u_char *)"\301\264\244\322\244\351\244\254\244\312",
 (u_char *)"\301\264\245\253\245\277\245\253\245\312",
 (u_char *)"\310\276\245\253\245\277\245\253\245\312",
 (u_char *)"\301\264\245\242\245\353\245\325\245\241",
 (u_char *)"\310\276\245\242\245\353\245\325\245\241",


 (u_char *)"  \312\321\264\271 \314\265\312\321 etc. \267\350\304\352 code  \310\276\245\242 \301\264\245\242 \310\276\245\253 \301\264\245\253 \244\253\244\312",
 (u_char *)"\244\275\244\316\302\276\241\241\241\241\241\241\241\241\241\241\241\241\241\241",

 (u_char *)"  \305\320\317\277 \276\303\265\356 Help sjrc \312\324\275\270  \272\306\300\334 \241\241\241\241 \241\241\241\241 \241\241\241\241 \241\241\241\241",

 (u_char *)"Help\241\241Mode",
 (u_char *)"  \245\254\245\244\245\311\241\246\245\351\245\244\245\363 %s \244\313\244\312\244\352\244\336\244\267\244\277 ",



 (u_char *)"ShiftJIS\241\241",
 (u_char *)"\241\241\243\305\243\325\243\303\241\241",
 (u_char *)"\243\305\243\325\243\303\312\344\275\365",
 (u_char *)"\241\241\243\312\243\311\243\323\241\241",
 (u_char *)"\243\312\243\311\243\323\312\344\275\365",
 (u_char *)"\241\241\241\241\266\350\305\300\241\241",
 (u_char *)"\266\350\305\300\241\241\312\344\275\365",



 (u_char *)"\305\320\317\277",
 (u_char *)" \310\317\260\317\244\362\273\330\304\352\244\267\244\306\244\257\244\300\244\265\244\244 ",
 (u_char *)" \264\301\273\372 [%s] : \306\311\244\337\306\376\316\317 ",
 (u_char *)" \264\301\273\372 [%s] : \306\311\244\337 [%s] : \311\312\273\354 [%s] (y/n)? ",
 (u_char *)" \264\301\273\372 [%s] : \306\311\244\337 [%s] (y/n)? ",
 (u_char *)" \312\270\313\241\276\360\312\363 : 1.\314\276\273\354 2.\314\276\273\372 3.\314\276\301\260 4.\303\317\314\276 5.\245\265\312\321\306\260\273\354",
 (u_char *)" \305\320\317\277\244\267\244\336\244\267\244\277 ",
 (u_char *)" \306\311\244\337\306\376\316\317 ",
 (u_char *)" 1",
 (u_char *)"y",
 (u_char *)" \306\261\260\354\244\316\303\261\270\354\244\254\244\271\244\307\244\313\302\270\272\337\244\267\244\306\244\244\244\336\244\271",
 (u_char *)" \244\263\244\316\306\311\244\337\312\270\273\372\316\363\244\307\244\317\305\320\317\277\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \244\263\244\316\264\301\273\372\312\270\273\372\316\363\244\307\244\317\305\320\317\277\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \244\263\244\354\260\312\276\345\306\261\244\270\306\311\244\337\244\307\244\317\305\320\317\277\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \244\263\244\354\260\312\276\345\245\346\241\274\245\266\274\255\275\361\244\313\244\317\305\320\317\277\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \245\346\241\274\245\266\274\255\275\361\244\254\245\355\245\303\245\257\244\265\244\354\244\306\244\244\244\336\244\271",
 (u_char *)" \245\267\245\271\245\306\245\340\245\250\245\351\241\274\244\307\305\320\317\277\244\307\244\255\244\336\244\273\244\363",
 (u_char *)",",


 (u_char *)"\276\303\265\356",
 (u_char *)" \264\301\273\372 [%s] : \306\311\244\337 [%s] : \311\312\273\354 [%s]\244\362\276\303\265\356\244\267\244\336\244\267\244\277 ",
 (u_char *)" \273\330\304\352\244\265\244\354\244\277\306\311\244\337\241\246\264\301\273\372\244\317\305\320\317\277\244\265\244\354\244\306\244\244\244\336\244\273\244\363",
 (u_char *)" \245\346\241\274\245\266\274\255\275\361\244\254\245\355\245\303\245\257\244\265\244\354\244\306\244\244\244\336\244\271",
 (u_char *)" \244\263\244\316\306\311\244\337\312\270\273\372\316\363\244\317\276\303\265\356\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \244\263\244\316\264\301\273\372\312\270\273\372\316\363\244\317\276\303\265\356\244\307\244\255\244\336\244\273\244\363",
 (u_char *)" \245\267\245\271\245\306\245\340\245\250\245\351\241\274\244\307\276\303\265\356\244\307\244\255\244\336\244\273\244\363",



 (u_char *)"\244\362\306\311\244\337\271\376\244\337\244\336\244\267\244\277",
 (u_char *)"\244\362\306\311\244\337\271\376\244\341\244\336\244\273\244\363\244\307\244\267\244\277",
 (u_char *)"\245\273\245\303\245\310\245\242\245\303\245\327\245\325\245\241\245\244\245\353\244\254\244\242\244\352\244\336\244\273\244\363 ",


 (u_char *)"Change weight (1-9) current weight %d ",



 (u_char *)"\272\306\300\334\302\263 ",
 (u_char *)" \245\265\241\274\245\320\314\276 [%s] \244\362\273\330\304\352\244\267\244\306\244\257\244\300\244\265\244\244 ",
 (u_char *)" \245\265\241\274\245\320 [%s] \244\313\300\334\302\263\244\267\244\336\244\267\244\277 ",
 (u_char *)"\245\265\241\274\245\320 [%s] \244\310\244\316\300\334\302\263\244\254\300\332\244\354\244\336\244\267\244\277",

 (u_char *)" ",
 (u_char *)"  ",
 (u_char *)" \302\263\244\261\244\336\244\271\244\253? y",


 (u_char *)"\245\265\241\274\245\320\241\274\244\254\273\340\244\313\244\336\244\267\244\277\241\243",
 (u_char *)"\245\265\241\274\245\320\241\274\244\313\300\334\302\263\272\321\244\307\244\271\241\243",
 (u_char *)"\245\265\241\274\245\320\241\274\244\330\244\316\300\334\302\263\244\313\274\272\307\324\244\267\244\336\244\267\244\277\241\243",
 (u_char *)"\245\341\245\244\245\363\274\255\275\361\245\325\245\241\245\244\245\353\244\362\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363\241\243",
 (u_char *)"\245\346\241\274\245\266\241\246\245\307\245\243\245\354\245\257\245\310\245\352\244\316\272\356\300\256\244\313\274\272\307\324\244\267\244\336\244\267\244\277\241\243",
 (u_char *)"\245\346\241\274\245\266\274\255\275\361\245\325\245\241\245\244\245\353\244\316\300\270\300\256\244\313\274\272\307\324\244\267\244\336\244\267\244\277\241\243",
 (u_char *)"\245\346\241\274\245\266\274\255\275\361\245\325\245\241\245\244\245\353\244\362\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363\241\243",
 (u_char *)"\263\330\275\254\245\325\245\241\245\244\245\353\244\316\300\270\300\256\244\313\274\272\307\324\244\267\244\336\244\267\244\277\241\243",
 (u_char *)"\263\330\275\254\245\325\245\241\245\244\245\353\244\362\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363\241\243",



 (u_char *)"\312\324\275\270 ",
 (u_char *)"\243\323\243\312\243\263\241\241\241\241\241\241\241\241\241\241\241\241\241\241", 

 (u_char *)" \312\324\275\270\312\270\273\372\316\363\244\254\304\271\244\271\244\256\244\336\244\271",
 (u_char *)"$$",


 (u_char *)"on",
 (u_char *)"off",


 (u_char *)"\314\276\273\354",
 (u_char *)"\314\276\273\372",
 (u_char *)"\314\276\301\260",
 (u_char *)"\303\317\314\276",
 (u_char *)"\245\265\312\321\306\260\273\354",


 (u_char *)"",
 (u_char *)"F1",
 (u_char *)"F2",
 (u_char *)"F3",
 (u_char *)"F4",
 (u_char *)"F5",
 (u_char *)"F6",
 (u_char *)"F7",
 (u_char *)"F8",
 (u_char *)"F9",
 (u_char *)"F10",
 (u_char *)"F11",
 (u_char *)"F12",
 (u_char *)"F13",
 (u_char *)"F14",
 (u_char *)"F15",
 (u_char *)"F16",
 (u_char *)"F17",
 (u_char *)"F18",
 (u_char *)"F19",
 (u_char *)"F20",
 (u_char *)"Select",
 (u_char *)"Kanji",
 (u_char *)"Cancel",
 (u_char *)"Execute",
 (u_char *)"Up",
 (u_char *)"Down",
 (u_char *)"Right",
 (u_char *)"Left",

 (u_char *)"start",
 (u_char *)"henkan",
 (u_char *)"muhen",
 (u_char *)"kettei",
 (u_char *)"hira",
 (u_char *)"halpha",
 (u_char *)"zalpha",
 (u_char *)"hkata",
 (u_char *)"zkata",
 (u_char *)"code",
 (u_char *)"etc",
 (u_char *)"right",
 (u_char *)"left",
 (u_char *)"kaku",
 (u_char *)"syuku",
 (u_char *)"edit",
 (u_char *)"toggle",
 (u_char *)"wrapback",
 (u_char *)"wrap",
 (u_char *)"sjrc",
 (u_char *)"buffer",
 (u_char *)"toroku",
 (u_char *)"syoukyo",
 (u_char *)"help",
 (u_char *)"ha-kettei",
 (u_char *)"flush",
 (u_char *)"debug",
 (u_char *)"cancel",
 (u_char *)"saihenkan",
 (u_char *)"ignore",
 (u_char *)"top",
 (u_char *)"end",
 (u_char *)"reconnect",
 (u_char *)"kill",


 (u_char *)"key",
 (u_char *)"escape",
 (u_char *)"sjxkey",
 (u_char *)"etckey",
 (u_char *)"intr",
 (u_char *)"bskey",
 (u_char *)"delkey",
 (u_char *)"setnormal",
 (u_char *)"throughnext",
 (u_char *)"initialmode",
 (u_char *)"helpmode",
 (u_char *)"defaultcode",
 (u_char *)"muhenkanmode",
 (u_char *)"muhenkaninedit",
 (u_char *)"muhenkantoggle",
 (u_char *)"guide",
 (u_char *)"forkshell",
 (u_char *)"execshell",
 (u_char *)"dictionary",
 (u_char *)"userdic",
 (u_char *)"maindic",
 (u_char *)"romaji",
 (u_char *)"bstudy",
 (u_char *)"silent",
 (u_char *)"flushafterconversion",
 (u_char *)"rkerrbell",
 (u_char *)"server",

 (u_char *)"shiftjis",
 (u_char *)"sjis",
 (u_char *)"euc",
 (u_char *)"euc2",
 (u_char *)"jis",
 (u_char *)"jis2",
 (u_char *)"kuten",
 (u_char *)"kuten2",

 (u_char *)"buff",
 (u_char *)"unbuff",
 (u_char *)"direct",

 (u_char *)"sj2",
 (u_char *)"shift",

 (u_char *)"function",

 (u_char *)"unbuffer",
 (u_char *)"funcetc"

};


#define CURMESSAGES 182
wchar16_t *WcMessages[CURMESSAGES];

#define CURWCVTKEY 29
struct wcvtkey wcvtkeytbl[CURWCVTKEY];



int 
init_wcvtkey()
{
	int i = 0;

	for (i = 0; i < CURWCVTKEY; i++) {
		wcvtkeytbl[i].key_seq[0] = 0;
	}
	wcvtkeytbl[0].key_word = WCF1;
	wcvtkeytbl[1].key_word = WCF2;
	wcvtkeytbl[2].key_word = WCF3;
	wcvtkeytbl[3].key_word = WCF4;
	wcvtkeytbl[4].key_word = WCF5;
	wcvtkeytbl[5].key_word = WCF6;
	wcvtkeytbl[6].key_word = WCF7;
	wcvtkeytbl[7].key_word = WCF8;
	wcvtkeytbl[8].key_word = WCF9;
	wcvtkeytbl[9].key_word = WCF10;
	wcvtkeytbl[10].key_word = WCF11;
	wcvtkeytbl[11].key_word = WCF12;
	wcvtkeytbl[12].key_word = WCF13;
	wcvtkeytbl[13].key_word = WCF14;
	wcvtkeytbl[14].key_word = WCF15;
	wcvtkeytbl[15].key_word = WCF16;
	wcvtkeytbl[16].key_word = WCF17;
	wcvtkeytbl[17].key_word = WCF18;
	wcvtkeytbl[18].key_word = WCF19;
	wcvtkeytbl[19].key_word = WCF20;
	wcvtkeytbl[20].key_word = WCSelect;
	wcvtkeytbl[21].key_word = WCKanji;
	wcvtkeytbl[22].key_word = WCCancel;
	wcvtkeytbl[23].key_word = WCExecute;
	wcvtkeytbl[24].key_word = WCUp;
	wcvtkeytbl[25].key_word = WCDown;
	wcvtkeytbl[26].key_word = WCRight;
	wcvtkeytbl[27].key_word = WCLeft;
	wcvtkeytbl[28].key_word = NULL;
}

wchar16_t *comstr[KEY_OTHER];



int
init_comstr()
{
	 comstr[0] = WCStart;     comstr[1] = WCHenkan;     comstr[2] = WCMuhen;
	 comstr[3] = WCKettei;    comstr[4] = WCHira;       comstr[5] = WCHalpha;
	 comstr[6] = WCZalpha;    comstr[7] = WCHkata;      comstr[8] = WCZkata;
	 comstr[9] = WCCode;      comstr[10] = WCEtc;       comstr[11] = WCRightl;
	 comstr[12] = WCLeftl;    comstr[13] = WCKaku;      comstr[14] = WCSyuku;
	 comstr[15] = WCEdit;     comstr[16] = WCToggle;    comstr[17] = WCWrapback;
	 comstr[18] = WCWrap;     comstr[19] = WCSjrc;      comstr[20] = WCBuffer;
	 comstr[21] = WCToroku;   comstr[22] = WCSyoukyo;   comstr[23] = WCHelp;
	 comstr[24] = WCHaKettei; comstr[25] = WCFlush;     comstr[26] = WCDebug;
	 comstr[27] = WCCancell;  comstr[28] = WCSaihenkan; comstr[29] = WCIgnore;
	 comstr[30] = WCTop;      comstr[31] = WCEnd;       comstr[32] = WCReconnect;
	 comstr[33] = WCKill;     comstr[34] = WCKara; 
}

struct valtbl hin_val[5];



int
init_hin_val()
{
	hin_val[0].keyword = WCMeishi; hin_val[0].ivalue = SJ3_H_NRMNOUN;
	hin_val[1].keyword = WCMyouji; hin_val[1].ivalue = SJ3_H_LNAME;
	hin_val[2].keyword = WCNamae;  hin_val[2].ivalue = SJ3_H_FNAME;
	hin_val[3].keyword = WCTimei;  hin_val[3].ivalue = SJ3_H_LOCNAME;
	hin_val[4].keyword = WCSahen;  hin_val[4].ivalue = SJ3_H_SILVERB;
}	


extern int   set_func(), set_etckeys(), set_intr(),
        set_delkey(), set_goto(), set_trap(), set_init_mode(),
	set_helpmenu(), set_defcode(), set_muhenkan(), set_muedit(),
        set_m_toggle(), set_guide(), set_forkshell(), set_bstudy(),
	set_silent(), set_flush_conversion(), set_rkebell(), set_server(),
	set_dict();
#define CURFUNCS 29
struct functbl funcs[CURFUNCS];



int
init_funcs()
{
	funcs[0].keyword = WCKey;               funcs[0].func = set_func;
	funcs[1].keyword = WCEscape;            funcs[1].func = set_func;
	funcs[2].keyword = WCEtckey;            funcs[2].func = set_etckeys;
	funcs[3].keyword = WCIntr;              funcs[3].func = set_intr;
	funcs[4].keyword = WCDelkey;            funcs[4].func = set_delkey;
	funcs[5].keyword = WCSetnormal;         funcs[5].func = set_goto;
	funcs[6].keyword = WCThroughnext;       funcs[6].func = set_trap;
	funcs[7].keyword = WCInitialmode;       funcs[7].func = set_init_mode;
	funcs[8].keyword = WCHelpmode;          funcs[8].func = set_helpmenu;
	funcs[9].keyword = WCDefaultcode;       funcs[9].func = set_defcode;
	funcs[10].keyword = WCMuhenkanmode;     funcs[10].func = set_muhenkan;
	funcs[11].keyword = WCMuhenkaninedit;   funcs[11].func = set_muedit;
	funcs[12].keyword = WCMuhenkantoggle;   funcs[12].func = set_m_toggle;
	funcs[13].keyword = WCGuide;            funcs[13].func = set_guide;
	funcs[14].keyword = WCForkshell;        funcs[14].func = set_forkshell;
	funcs[15].keyword = WCExecshell;        funcs[15].func = set_forkshell;
	funcs[16].keyword = WCBstudy;           funcs[16].func = set_bstudy;
	funcs[17].keyword = WCSilent;           funcs[17].func = set_silent;
	funcs[18].keyword = WCFlushafterconversion; funcs[18].func = set_flush_conversion;
	funcs[19].keyword = WCRkerrbell;        funcs[19].func = set_rkebell;
	funcs[20].keyword = WCServer;           funcs[20].func = set_server;
	funcs[21].keyword = WCDictionary;       funcs[21].func = set_dict;
	funcs[22].keyword = NULL;               funcs[22].func = NULL;
}

struct valtbl mode_val[5];  



int
init_mode_val()
{
	mode_val[0].keyword = WCHalpha; mode_val[0].ivalue = MODE_HALPHA;
	mode_val[1].keyword = WCZalpha; mode_val[1].ivalue = MODE_ZALPHA;
	mode_val[2].keyword = WCHkata;  mode_val[2].ivalue = MODE_HKATA;
	mode_val[3].keyword = WCZkata;  mode_val[3].ivalue = MODE_ZKATA;
	mode_val[4].keyword = WCHira;   mode_val[4].ivalue = MODE_ZHIRA;
}

struct valtbl code_val[8];



int
init_code_val()
{
	code_val[0].keyword = WCShiftjis; code_val[0].ivalue = CODE_SJIS;
	code_val[1].keyword = WCSjis;     code_val[1].ivalue = CODE_SJIS;
	code_val[2].keyword = WCEuc;      code_val[2].ivalue = CODE_EUC;
	code_val[3].keyword = WCEuc2;     code_val[3].ivalue = CODE_EUC2;
	code_val[4].keyword = WCJis;      code_val[4].ivalue = CODE_JIS;
	code_val[5].keyword = WCJis2;     code_val[5].ivalue = CODE_JIS2;
	code_val[6].keyword = WCKuten;    code_val[6].ivalue = CODE_KUTEN;
	code_val[7].keyword = WCKuten2;   code_val[7].ivalue = CODE_KUTEN2;
}

int
init_messages()
{
#define LOCALELEN 20
	char cur_loc[LOCALELEN];
	char *loc;
        short message_len[CURMESSAGES];
	int ret;
	int i;
        int all_len = 0;
	int saved_locale = 0;
	extern int current_locale;

	if (current_locale != LC_CTYPE_EUC) {
		saved_locale = current_locale;
		current_locale = LC_CTYPE_EUC;
#ifdef __sony_news
		strncpy((char *)cur_loc, setlocale(LC_CTYPE, NULL), LOCALELEN);
		loc = setlocale(LC_CTYPE, "ja_JP.EUC");
#endif
	}
	for (i = 0; i < CURMESSAGES; i++) {
		message_len[i] = jstrlen((char *)EucMessages[i]) + 1;
		all_len += message_len[i];
	} 

	WcMessages[0] = (wchar16_t *) malloc(all_len * sizeof(wchar16_t));
	if (WcMessages[0] == NULL) 
	  goto Fail;

	ret = (int) mbstowcs(WcMessages[0], (char *)EucMessages[0], message_len[0]);
	if (ret < -1) 
	  goto Fail;
	for (i = 1; i < CURMESSAGES; i++) {
		WcMessages[i] = WcMessages[i-1] + message_len[i-1];
		ret = mbstowcs(WcMessages[i], (char *)EucMessages[i], message_len[i]);
		if (ret < -1)
		  goto Fail;
	}

	init_wcvtkey();
	init_comstr();
	init_hin_val();
	init_funcs();
	init_mode_val();
	init_code_val();

	if (saved_locale) {
		current_locale = saved_locale;
#ifdef __sony_news
	        setlocale(LC_CTYPE, cur_loc);
#endif
	}
	return TRUE;
Fail:
	if (saved_locale) {
		current_locale = saved_locale;
#ifdef __sony_news
	        setlocale(LC_CTYPE, cur_loc);
#endif
	}
	return FALSE;
}

