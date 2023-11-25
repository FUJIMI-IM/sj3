/*
 * Copyright (c) 1991-1994  Sony Corporation
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
 * $SonyRCSfile: fzkyomi.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:48 $
 */



#include "sj_const.h"
#include "sj_rename.h"
#include "sj_typedef.h"
#include "sj_yomi.h"
#include "sj_right.h"
#include "sj_left.h"

static	Uchar	fzk_wo[] = {
(0<<5)+0,			L_WO,      R_WO,
(0<<5)+0,			L_WO_S,	   R_WO_S,
0xff
};

static	Uchar	fzk_xtu[] = {
(0<<5) + 1,	_KE,		L_KKE,	   R_SHUUJO,
(0<<5) + 3,	_TA,_XTU,_TE,	L_TTATTE,  R_TTATTE,
(1<<5) + 1,	_RA,		L_TTARA,   R_TTARA,
(2<<5) + 2,	_SI, _I,	L_TTARASII, R_KSHUUSI,
(3<<5) + 2,	_KA, _XTU,	L_TTARASII, R_ONBIN1,
(4<<5) + 2,	_RO, _U,	L_TTARASII, R_KAROU,
(3<<5) + 1,	_KU,		L_TTARASII, R_KRENYOU,
(3<<5) + 2,	_KE, _RE,	L_TTARASII, R_KATEI1,
(3<<5) + 1,	_SA,		L_TTARASII, R_FZKMEISI,
(0<<5) + 1,	_TE,		L_TTE1,    R_TTE1,
(1<<5) + 0,			L_TTE2,    R_TTE2,
(0<<5) + 3,	_PA,_NA,_SI,	L_PPANASI, R_PPANASI,
(0<<5) + 2,	_PO,_I,		L_PPOI,    R_PPOI,
(1<<5) + 2,	_KA,_XTU,	L_PPOI,	   R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_PPOI,	   R_KAROU,
(1<<5) + 1,	_KU,		L_PPOI,	   R_KRENYOU,
(1<<5) + 2,	_KE,_RE,	L_PPOI,	   R_KATEI1,
0xff
};

static	Uchar	fzk_a[] = {
(0<<5) + 1,	_GE,		L_YOUGEN, R_AGE,
(1<<5) + 1,	_YO,		L_YOUGEN, R_MEIREI2,
(1<<5) + 1,	_RU,		L_YOUGEN, R_SHUUSI,
(1<<5) + 1,	_RE,		L_YOUGEN, R_KATEI3,
(1<<5) + 1,	_RO,		L_YOUGEN, R_MEIREI1,
(0<<5) + 1,	_XTU,		L_ARU,    R_AXTU,
(0<<5) + 1,	_RA,		L_ARA,    R_MIZEN2,
(0<<5) + 1,	_RI,		L_ARI1,   R_ARI1,
(1<<5) + 0,			L_ARI2,   R_ARI2,
(1<<5) + 0,			L_ARI3,   R_ARI3,
(0<<5) + 1,	_RU,		L_ARU,    R_SHUUSI,
(0<<5) + 1,	_RE,		L_ARE1,   R_KAMEI,
(1<<5) + 0,			L_ARE2,   R_KATEI2,
(1<<5) + 3,	_BA,_KO,_SO,	L_ARE1,   R_CUT,
(0<<5) + 2,	_RO,_U,		L_AROU,   R_AROU,
0xff
};

static	Uchar	fzk_i[] = {
(0<<5) + 0,			L_IRU,     R_I,
(0<<5) + 1,	_I,		L_II,	   R_SHUUSI,
(0<<5) + 1,	_KA,		L_YOUGEN,  R_MIZEN5,
(0<<5) + 1,	_KI,		L_YOUGEN,  R_IKI,
(0<<5) + 1, 	_KU,		L_YOUGEN,  R_SHUUSI,
(0<<5) + 1,	_KE,		L_YOUGEN,  R_IKE,
(0<<5) + 2,	_KO,_U,		L_YOUGEN,  R_AROU,
(0<<5) + 2,	_TA,_SI,	L_ITASI,   R_ITASI,
(1<<5) + 2,	_DA,_I,		L_ITADAKU, R_ONBIN2,
(2<<5) + 1,	_KA,		L_ITADAKU, R_MIZEN4,
(2<<5) + 1,	_KI,		L_ITADAKU, R_ITADAKI,
(2<<5) + 1,	_KU,		L_ITADAKU, R_SHUUSI,
(2<<5) + 1,	_KE,		L_ITADAKU, R_ITADAKE,
(2<<5) + 2,	_KO,_U,		L_ITADAKU, R_AROU,
(0<<5) + 1,	_XTU,		L_YOUGEN,  R_ONBIN8,
(0<<5) + 1,	_YO,		L_IRU,     R_MEIREI2,
(0<<5) + 1,	_RU,		L_IRU,     R_SHUUSI,
(0<<5) + 1,	_RE,		L_IRU,     R_KATEI2,
(0<<5) + 1,	_RO,		L_IRU,     R_MEIREI1,
0xff
};

static	Uchar	fzk_u[] = {
(0<<5) + 1,	_E,		L_UE,      R_UE,
(0<<5) + 1,	_TI,		L_UTI,	   R_UTI,
(0<<5) + 1,	_RU,		L_ERU,     R_SHUUSI,
0xff
};

static	Uchar	fzk_e[] = {
(0<<5) + 0,			L_ERU,     R_E,
(0<<5) + 1,	_RU,		L_ERU,	   R_SHUUSI,
(0<<5) + 1,	_RE,		L_ERU,     R_KATEI3,
0xff
};

static	Uchar	fzk_o[] = {
(0<<5) + 1,	_I,		L_YOUGEN,  R_ONBIN8,
(1<<5) + 1,	_TE,		L_OITE,    R_OITE,
(0<<5) + 1,	_KA,		L_YOUGEN,  R_MIZEN5,
(0<<5) + 1,	_KI,		L_YOUGEN,  R_IKI,
(0<<5) + 1,	_KU,		L_YOUGEN,  R_SHUUSI,
(0<<5) + 1,	_KE,		L_YOUGEN,  R_IKE,
(1<<5) + 1,	_RU,		L_OKERU,   R_OKERU,
(0<<5) + 2,	_KO,_U,		L_YOUGEN,  R_AROU,
(0<<5) + 1,	_RA,		L_YOUGEN,  R_MIZEN2,
(0<<5) + 1,	_RI,		L_ORI,     R_ORI1,
(1<<5) + 0,			L_YOUGEN,  R_ORI2,
(0<<5) + 1,	_RE,		L_YOUGEN,  R_KAMEI,
0xff
};

static	Uchar	fzk_ka[] = {
(0<<5) + 0 + 8,			L_KA1,     R_KA1,
(0<<5) + 0 + 8,			L_KA2,     R_KA2,
(0<<5) + 1,	_I,		L_KAI,	   R_SHUUJO,
(0<<5) + 1,	_KE,		L_KAKERU,  R_KAKE,
(1<<5) + 1,	_RU,		L_KAKERU,  R_SHUUSI,
(1<<5) + 1,	_RE,		L_KAKERU,  R_KATEI3,
(0<<5) + 2,	_SI,_RA,	L_KASIRA,  R_SHUUJO,
(2<<5) + 1,	_NE,		L_KASIRA,  R_SHUUJO,
(0<<5) + 3,	_TA,_GA,_TA,	L_KATAGATA,R_KATAGATA,
(0<<5) + 3,	_DO,_U,_KA,	L_KADOUKA, R_KADOUKA,
(0<<5) + 1,	_NA,		L_KANA,    R_SHUUJO,
(0<<5) + 1,	_NE,		L_KANE,    R_SHUUJO,
(1<<5) + 0,			L_KANERU,  R_KANE2,
(1<<5) + 1,	_RU,		L_KANERU,  R_SHUUSI,
(1<<5) + 1,	_RE,		L_KANERU,  R_KATEI3,
(0<<5) + 3,	_MA,_TA,_HA,	L_KA1,	   R_CUT,
(0<<5) + 1,	_MO,		L_KAMO,    R_KAMO,
(0<<5) + 1,	_RA,		L_KARA1,   R_KARA1,
(1<<5) + 0,			L_KARA2,   R_KARA2,
(1<<5) + 1,	_ZU,		L_KARAZU,  R_ZU,
(1<<5) + 1,	_NU,		L_KARAZU,  R_NU,
0xff
};

static	Uchar	fzk_ga[] = {
(0<<5) + 0,			L_GA1,     R_GA1,
(0<<5) + 0,			L_GA2,     R_GA2,
(0<<5) + 2,	_TA,_I,		L_GATAI,   R_KSHUUSI,
(1<<5) + 2,	_KA,_XTU,	L_GATAI,   R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_GATAI,   R_KAROU,
(1<<5) + 1,	_KU,		L_GATAI,   R_GATAKU,
(1<<5) + 2,	_KE,_RE,	L_GATAI,   R_KATEI2,
(0<<5) + 1,	_TI,		L_GATIDA,  R_GATI,
(1<<5) + 1,	_DA,		L_GATIDA,  R_DSHUUSI1,
(2<<5) + 1,	_XTU,		L_GATIDA,  R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_GATIDA,  R_DAROU,
(1<<5) + 1, 	_DE,		L_GATIDA,  R_DRENYOU1,
(1<<5) + 1,	_NA,		L_GATIDA,  R_DRENTAI1,
(2<<5) + 1,	_RA,		L_GATIDA,  R_KATEI1,
(1<<5) + 1,	_NI,		L_GATIDA,  R_DRENYOU2,
(0<<5) + 2,	_TE,_RA,	L_GATERA,  R_GATERA,
0xff
};

static	Uchar	fzk_ki[] = {
(0<<5) + 0,			L_YOUGEN,  R_FZKKI,
(0<<5) + 1,	_RI,		L_KIRI,    R_KIRI,
0xff
};

static	Uchar	fzk_ku[] = {
(0<<5) + 2,	_SE,_NI,	L_KUSENI,   R_KUSENI,
(0<<5) + 3,	_DA,_SA,_I,	L_KUDASARU, R_KUDASAI,
(2<<5) + 1,	_XTU,		L_KUDASARU, R_ONBIN6,
(2<<5) + 1,	_RA,		L_KUDASARU, R_MIZEN4,
(2<<5) + 1,	_RI,		L_KUDASARU, R_ITADAKI,
(2<<5) + 1,	_RU,		L_KUDASARU, R_SHUUSI,
(2<<5) + 1,	_RE,		L_KUDASARU, R_KATEI3,
(0<<5) + 2,	_RA,_I,		L_KURAI,    R_KURAI,
(0<<5) + 1,	_RU,		L_YOUGEN,   R_SHUUSI,
(0<<5) + 1,	_RE,		L_YOUGEN,   R_KURE,
(1<<5) + 1,	_RU,		L_YOUGEN,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_YOUGEN,   R_KATEI3,
0xff
};

static	Uchar	fzk_gu[] = {
(0<<5) + 2,	_RA,_I,		L_KURAI,    R_KURAI,
(0<<5) + 2,	_RU,_MI,	L_GURUMI,   R_GURUMI,
0xff
};

static	Uchar	fzk_ke[] = {
(0<<5) + 1,	_DO,		L_KEDO,     R_KEDO,
(1<<5) + 1,	_MO,		L_KEDO,     R_KEDO,
(0<<5) + 2,	_RE,_DO,	L_KEDO,     R_KEDO,
(2<<5) + 1,	_MO,		L_KEDO,     R_KEDO,
0xff
};

static	Uchar	fzk_ko[] = {
(0<<5) + 0,			L_YOUGEN,   R_KO,
(0<<5) + 1,	_I,		L_YOUGEN,   R_MEIREI1,
(0<<5) + 1,	_SO,		L_KOSO,     R_KOSO,
(0<<5) + 1,	_TO,		L_KOTO,     R_KOTO,
0xff
};

static	Uchar	fzk_go[] = {
(0<<5) + 2,	_ZA,_I,		L_GOZAI,    R_GOZAI,
(0<<5) + 2,	_TO,_KI,	L_GOTOSI,   R_GOTOKI,
(1<<5) + 1,	_KU,		L_GOTOSI,   R_GOTOKU,
(1<<5) + 1,	_SI,		L_GOTOSI,   R_GOTOSI,
0xff
};

static	Uchar	fzk_sa[] = {
(0<<5) + 0,			L_SA1,      R_MIZEN1,
(0<<5) + 0,			L_SA2,      R_SHUUJO,
(0<<5) + 1,	_E,		L_SAE,      R_SAE,
(0<<5) + 1,	_SE,		L_SASERU,   R_SASE,
(1<<5) + 1,	_YO,		L_SASERU,   R_MEIREI2,
(1<<5) + 1,	_RU,		L_SASERU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_SASERU,   R_KATEI2,
(1<<5) + 1,	_RO,		L_SASERU,   R_MEIREI1,
0xff
};

static	Uchar	fzk_za[] = {
(0<<5) + 1,	_RU,		L_ZARU,     R_ZARU,
(0<<5) + 1,	_RE,		L_ZARU,     R_KATEI2,
0xff
};

static	Uchar	fzk_si[] = {
(0<<5) + 0,			L_SI1,      R_SI1,
(0<<5) + 0,			L_SI2,      R_SI2,
(0<<5) + 0,			L_N_SI,	    R_N_SI,
(0<<5) + 1,	_KA,		L_SIKA,     R_SIKA,
(0<<5) + 1,	_TE,		L_SITE,     R_SITE,
(0<<5) + 2,	_MA,_I,		L_YOUGEN,   R_IKI,
(1<<5) + 1,	_U,		L_YOUGEN,   R_SHUUSI,
(1<<5) + 1,	_E,		L_YOUGEN,   R_IKE,
(1<<5) + 2,	_O,_U,		L_YOUGEN,   R_AROU,
(1<<5) + 1,	_XTU,		L_YOUGEN,   R_ONBIN6,
(1<<5) + 1,	_WA,		L_YOUGEN,   R_MIZEN5,
(0<<5) + 1,	_RO,		L_SIRO,	    R_MEIREI1,
0xff
};

static	Uchar	fzk_ji[] = {
(0<<5) + 2,	_MA,_I,		L_JIMAU,    R_IKI,
(1<<5) + 1,	_U,		L_JIMAU,    R_SHUUSI,
(1<<5) + 1,	_E,		L_JIMAU,    R_IKE,
(1<<5) + 2,	_O,_U,		L_JIMAU,    R_AROU,
(1<<5) + 1,	_XTU,		L_JIMAU,    R_ONBIN6,
(1<<5) + 1,	_WA,		L_JIMAU,    R_MIZEN5,
(0<<5) + 1,	_XYA,		L_JYA,	    R_JYA,
(1<<5) + 1,	_I,		L_JYAU,     R_IKI,
(1<<5) + 1,	_U,		L_JYAU,     R_SHUUSI,
(1<<5) + 1,	_E,		L_JYAU,     R_IKE,
(1<<5) + 2,	_O,_U,		L_JYAU,     R_AROU,
(1<<5) + 1,	_XTU,		L_JYAU,     R_ONBIN6,
(1<<5) + 1,	_WA,		L_JYAU,	    R_MIZEN5,
0xff
};

static	Uchar	fzk_su[] = {
(0<<5) + 1,	_GI,		L_SUGIRU,   R_SUGI,
(1<<5) + 1,	_RU,		L_SUGIRU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_SUGIRU,   R_KATEI2,
(0<<5) + 3,	_BE,_KA,_RA,	L_SUBESI,   R_BEKARA,
(1<<5) + 1,	_KI,		L_SUBESI,   R_BEKI,
(1<<5) + 1,	_KU,		L_SUBESI,   R_BEKU,
(1<<5) + 1,	_SI,		L_SUBESI,   R_BESI,
(0<<5) + 1,	_RA,		L_SURA,     R_SURA,
(0<<5) + 1,	_RU,		L_SURU,     R_SHUUSI,
(0<<5) + 1,	_RE,		L_SURE,     R_KATEI2,
0xff
};

static	Uchar	fzk_zu[] = {
(0<<5) + 0,			L_ZU,       R_ZU,
(0<<5) + 1,	_TU,		L_ZUTU,	    R_ZUTU,
(0<<5) + 2,	_NI,_I,		L_ZU,	    R_I,
(2<<5) + 1,	_RU,		L_ZU,       R_SHUUSI,
(0<<5) + 2,	_NN,_BA,	L_ZUNBA,    R_CUT,
0xff
    };

static	Uchar	fzk_se[] = {
(0<<5) + 0,			L_SE,       R_MIZEN2,
(0<<5) + 0,			L_SERU,     R_SASE,
(0<<5) + 1,	_YO,		L_SEYO,     R_MEIREI2,
(1<<5) + 0,			L_SERU,     R_MEIREI2,
(0<<5) + 1,	_RU,		L_SERU,     R_SHUUSI,
(0<<5) + 1,	_RE,		L_SERU,     R_KATEI2,
(0<<5) + 1,	_RO,		L_SERU,     R_MEIREI1,
0xff
};

static	Uchar	fzk_ze[] = {
(0<<5) + 0,			L_ZE,	    R_SHUUJO,
0xff
};

static	Uchar	fzk_so[] = {
(0<<5) + 1,	_U,		L_SOUDA,    R_SOU,
(1<<5) + 1,	_DA,		L_SOUDA,    R_SOUDA,
(2<<5) + 1,	_XTU,		L_YOUTAI,   R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_YOUTAI,   R_DAROU,
(1<<5) + 1,	_DE,		L_SOUDA,    R_SOUDE,
(2<<5) + 1,	_SI,		L_SOUDA,    R_DESHI,
(3<<5) + 1,	_XYO,		L_YOUTAI,   R_SHO,
(4<<5) + 1,	_U,		L_YOUTAI,   R_DESHOU,
(2<<5) + 1,	_SU,		L_SOUDA,    R_SOUDESU,
(1<<5) + 1,	_NA,		L_DENBUN,   R_SOUNA1,
(2<<5) + 0,			L_YOUTAI,   R_SOUNA2,
(2<<5) + 1,	_RA,		L_SOUDA,    R_KATEI1,
(1<<5) + 1,	_NI,		L_YOUTAI,   R_SOUNI,
0xff
};

static	Uchar	fzk_zo[] = {
(0<<5) + 0,			L_ZO,       R_SHUUJO,
0xff
};

static	Uchar	fzk_ta[] = {
(0<<5) + 0,			L_TA,       R_TA,
(0<<5) + 1,	_I,		L_TAI,      R_TAI,
(0<<5) + 2,	_KA,_XTU,	L_TAI,      R_ONBIN1,
(1<<5) + 2,	_RO,_U,		L_TAI,      R_KAROU,
(0<<5) + 2,	_GA,_XTU,	L_TAGARU,   R_ONBIN6,
(1<<5) + 1,	_RA,		L_TAGARU,   R_TAGARA,
(1<<5) + 1,	_RI,		L_TAGARU,   R_TAGARI,
(1<<5) + 1,	_RU,		L_TAGARU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_TAGARU,   R_KATEI3,
(1<<5) + 2,	_RO,_U,		L_TAGARU,   R_KAROU,
(0<<5) + 1,	_KU,		L_TAI,      R_TAKU,
(0<<5) + 2,	_KE,_RE,	L_TAI,      R_KATEI3,
(0<<5) + 2,	_MA,_I,		L_TAMAU,    R_IKI,
(1<<5) + 1,	_U,		L_TAMAU,    R_SHUUSI,
(1<<5) + 1,	_E,		L_TAMAU,    R_KAMEI,
(1<<5) + 1,	_XTU,		L_TAMAU,    R_ONBIN6,
(1<<5) + 1,	_WA,		L_TAMAU,    R_MIZEN5,
(0<<5) + 1,	_ME,		L_TAME,     R_TAME,
(0<<5) + 1,	_RA,		L_TA,       R_KATEI1,
(0<<5) + 1,	_RI,		L_TA,       R_TARI,
(0<<5) + 1,	_RU,		L_TARU,	    R_CUT,
(0<<5) + 2,	_RO,_U,		L_TA,       R_KAROU,
0xff
};

static	Uchar	fzk_da[] = {
(0<<5) + 0,			L_DA1,	    R_TA,
(0<<5) + 0,			L_DA2,      R_DSHUUSI1,
(0<<5) + 1,	_KE,		L_DAKE,     R_DAKE,
(0<<5) + 1,	_XTU,		L_DAXTU,    R_ONBIN1,
(0<<5) + 1,	_NO,		L_DANO,     R_DANO,
(0<<5) + 1,	_RA,		L_DA1,      R_KATEI1,
(1<<5) + 1,	_KE,		L_DARAKE,   R_DARAKE,
(0<<5) + 1,	_RI,		L_DA1,      R_TARI,
(0<<5) + 2,	_RO,_U,		L_DAROU,    R_DAROU,
0xff
};

static	Uchar	fzk_ti[] = {
(0<<5) + 2,	_MA,_I,		L_TIMAU,    R_IKI,
(1<<5) + 1,	_U,		L_TIMAU,    R_SHUUSI,
(1<<5) + 1,	_E,		L_TIMAU,    R_IKE,
(1<<5) + 2,	_O,_U,		L_TIMAU,    R_AROU,
(1<<5) + 1,	_XTU,		L_TIMAU,    R_ONBIN6,
(1<<5) + 1,	_WA,		L_TIMAU,    R_MIZEN5,
(0<<5) + 1,	_XYA,		L_TYA,      R_CUT,
(1<<5) + 1,	_I,		L_TYAU,     R_IKI,
(1<<5) + 1,	_U,		L_TYAU,     R_SHUUSI,
(1<<5) + 1,	_E,		L_TYAU,	    R_IKE,
(1<<5) + 2,	_O,_U,		L_TYAU,     R_AROU,
(1<<5) + 1,	_XTU,		L_TYAU,     R_ONBIN6,
(1<<5) + 1,	_WA,		L_TYAU,	    R_MIZEN5,
0xff
};

static	Uchar	fzk_di[] = {
(0<<5) + 1,	_XYA,		L_JYA,      R_JYA,
0xff
};

static	Uchar	fzk_tu[] = {
(0<<5) + 2,	_I,_TE,		L_TUITE,    R_TUITE,
(0<<5) + 1,	_KI,		L_TUKI,     R_TUKI,
(0<<5) + 1,	_TU,		L_TUTU,     R_TUTU,
(0<<5) + 2,	_MO,_RI,	L_TUMORI,   R_TUMORI,
0xff
};

static	Uchar	fzk_te[] = {
(0<<5) + 0,			L_TE1,      R_TE1,
(0<<5) + 0,			L_TE2,      R_TE2,
(0<<5) + 1,	_RU,		L_TERU,     R_TERU,
0xff
};

static	Uchar	fzk_de[] = {
(0<<5) + 0,			L_DE,       R_DE,
(0<<5) + 0,			L_DA1,      R_TE2,
(0<<5) + 3,	_A,_RO,_U,	L_DEAROU,   R_AROU,
(0<<5) + 1,	_KI,		L_DEKIRU,   R_DEKI,
(1<<5) + 1,	_RU,		L_DEKIRU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_DEKIRU,   R_KATEI3,
(0<<5) + 1,	_SI,		L_DESI,     R_DESHI,
(1<<5) + 1,	_XYO,		L_DESHO,    R_SHO,
(2<<5) + 1,	_U,		L_DESHO,    R_DESHOU,
(0<<5) + 1,	_SU,		L_DESU,     R_DESU,
(0<<5) + 1,	_HA,		L_DEHA,	    R_DEHA,
(0<<5) + 1,	_RU,		L_DERU,     R_TERU,
0xff
};

static	Uchar	fzk_to[] = {
(0<<5) + 0,			L_TO1,      R_TO1,
(0<<5) + 0,			L_TO2,      R_TO2,
(0<<5) + 1,	_I,		L_TE2,      R_ONBIN8,
(1<<5) + 1,	_U,		L_TOIU,     R_TOIU,
(0<<5) + 2,	_O,_RI,		L_TOORI,    R_TOORI,
(0<<5) + 1,	_KA,		L_TE2,      R_MIZEN5,
(0<<5) + 1,	_KI,		L_TOKI,     R_TOKI,
(1<<5) + 0,			L_TE2,      R_IKI,
(0<<5) + 1,	_KU,		L_TE2,      R_SHUUSI,
(0<<5) + 1,	_KE,		L_TE2,      R_IKE,
(0<<5) + 1,	_KO,		L_TOKORO,   R_TOKORO,
(1<<5) + 1,	_U,		L_TE2,      R_AROU,
(1<<5) + 1,	_RO,		L_TOKORO,   R_TOKORO,
(0<<5) + 2,	_XTU,_TE,	L_TOTTE,    R_TOTTE,
(0<<5) + 1,	_TE,		L_TOTE,     R_TOTE,
(0<<5) + 3,	_HA,_I,_E,	L_TOHAIE,   R_TOHAIE,
(0<<5) + 1,	_MO,		L_TOMO,     R_TOMO,
0xff
};

static	Uchar	fzk_do[] = {
(0<<5) + 0,			L_DO,       R_DO,
(0<<5) + 1,	_I,		L_DA1,      R_ONBIN8,
(0<<5) + 2,	_O,_RI,		L_DOORI,    R_DOORI,
(0<<5) + 1,	_KA,		L_DA1,      R_MIZEN5,
(0<<5) + 1,	_KI,		L_DA1,      R_IKI,
(0<<5) + 1,	_KU,		L_DA1,      R_SHUUSI,
(0<<5) + 1,	_KE,		L_DA1,      R_IKE,
(0<<5) + 2,	_KO,_U,		L_DA1,      R_AROU,
(1<<5) + 1,	_RO,		L_DOKORO,   R_DOKORO,
(2<<5) + 1,	_KA,		L_DOKOROKA, R_DOKOROKA,
(0<<5) + 1,	_MO,		L_DOMO,     R_DOMO,
0xff
};

static	Uchar	fzk_na[] = {
(0<<5) + 0,			L_NA1,      R_SHUUJO,
(0<<5) + 0,			L_NA2,      R_NA2,
(0<<5) + 1,	_XA,		L_NAA,      R_SHUUJO,
(0<<5) + 1,	_A,		L_NAA,      R_SHUUJO,
(0<<5) + 1,	_I,		L_NAI,	    R_NAI,
(1<<5) + 1,	_DE,		L_NAI,      R_NAIDE,
(0<<5) + 2,	_KA,_XTU,	L_NAI,      R_ONBIN1,
(1<<5) + 2,	_RO,_U,		L_NAI,      R_KAROU,
(0<<5) + 2,	_GA,_RA,	L_NAGARA,   R_NAGARA,
(2<<5) + 0,			L_NAGARA2,  R_NAGARA2,
(0<<5) + 2,	_KI,_XYA,	L_NAI,	    R_CUT,
(0<<5) + 1,	_KU,		L_NAI,      R_NAKU,
(0<<5) + 2,	_KE,_RE,	L_NAI,      R_KATEI3,
(0<<5) + 2,	_SA,_I,		L_NASAI,    R_NASAI,
(1<<5) + 1,	_RA,		L_NASARU,   R_MIZEN4,
(1<<5) + 1,	_RI,		L_NASARU,   R_ITADAKI,
(1<<5) + 1,	_RU,		L_NASARU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_NASARU,   R_KATEI3,
(2<<5) + 1,	_RU,		L_NASARU,   R_SHUUSI,
(0<<5) + 1,	_ZO,		L_NAZO,     R_NAZO,
(0<<5) + 1,	_XTU,		L_NARU,     R_ONBIN2,
(0<<5) + 1,	_DO,		L_NADO,     R_NADO,
(0<<5) + 1,	_YO,		L_NA1,      R_SHUUJO,
(0<<5) + 1,	_RA,		L_NARA,     R_KATEI1,
(1<<5) + 0,			L_NARU,     R_MIZEN5,
(0<<5) + 1,	_RI,		L_NARI,     R_NARI,
(1<<5) + 0,			L_NARU,	    R_IKI,
(0<<5) + 1,	_RU,		L_NARU,     R_SHUUSI,
(0<<5) + 1,	_RE,		L_NARU,     R_IKE,
(0<<5) + 2,	_RO,_U,		L_NARU,     R_AROU,
(0<<5) + 2,	_NN,_KA,	L_NANKA,    R_NANKA,
(1<<5) + 1,	_ZO,		L_NANZO,    R_NANZO,
(1<<5) + 1,	_TE,		L_NANTE,    R_NANTE,
0xff
};

static	Uchar	fzk_ni[] = {
(0<<5) + 0,			L_NI1,      R_NI1,
(0<<5) + 0,			L_NI2,      R_NI2,
(0<<5) + 0,			L_NI3,      R_NI3,
(0<<5) + 2,	_KU,_I,		L_NIKUI,    R_KSHUUSI,
(1<<5) + 2,	_KA,_XTU,	L_NIKUI,    R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_NIKUI,    R_KAROU,
(1<<5) + 1,	_KU,		L_NIKUI,    R_TAKU,
(1<<5) + 2,	_KE,_RE,	L_NIKUI,    R_KATEI3,
(1<<5) + 1,	_SA,		L_NIKUI,    R_FZKMEISI,
(0<<5) + 1,	_TE,		L_NITE,     R_NITE,
(0<<5) + 2,	_DE,_MO,	L_NI1,	    R_MO1,
(0<<5) + 1,	_MO,		L_NIMO,     R_NIMO,
0xff
};

static	Uchar	fzk_nu[] = {
(0<<5) + 0,			L_NU,       R_NU,
0xff
};

static	Uchar	fzk_ne[] = {
(0<<5) + 0,			L_NE1,      R_SHUUJO,
(0<<5) + 0,			L_NE2,      R_KATEI2,
(0<<5) + 1,	_E,		L_NEE,      R_SHUUJO,
0xff
};

static	Uchar	fzk_no[] = {
(0<<5) + 0,			L_NO1,      R_NO1,
(0<<5) + 0,			L_NO2,      R_NO2,
(0<<5) + 1,	_DE,		L_NODE,     R_NODE,
(0<<5) + 1,	_NI,		L_NONI,     R_NONI,
(0<<5) + 1,	_MI,		L_NOMI,     R_NOMI,
(1<<5) + 3,	_NA,_RA,_ZU,	L_NOMINRZ,  R_NOMINRZ,
0xff
};

static	Uchar	fzk_ha[] = {
(0<<5) + 0,			L_HA,       R_HA,
(0<<5) + 1,	_SI,		L_HA,	    R_HASI,
(0<<5) + 1,	_ZU,		L_HAZU,     R_HAZU,
0xff
};

static	Uchar	fzk_ba[] = {
(0<<5) + 0,			L_BA,       R_BA,
(0<<5) + 2,	_KA,_SI,	L_BAKARI,   R_BAKARI,
(1<<5) + 1,	_RI,		L_BAKARI,   R_BAKARI,
(0<<5) + 3,	_XTU,_KA,_SI,	L_BAKARI,   R_BAKARI,
(2<<5) + 1,	_RI,		L_BAKARI,   R_BAKARI,
0xff
};

static	Uchar	fzk_hu[] = {
(0<<5) + 2,	_U,_DA,		L_FUUDA,    R_SOUDA,
(2<<5) + 1,	_XTU,		L_FUUDA,    R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_FUUDA,    R_DAROU,
(1<<5) + 1,	_DE,		L_FUUDA,    R_SOUDE,
(2<<5) + 1,	_SI,		L_FUUDA,    R_DESHI,
(3<<5) + 1,	_XYO,		L_FUUDA,    R_SHO,
(4<<5) + 1,	_U,		L_FUUDA,    R_DESHOU,
(2<<5) + 1,	_SU,		L_FUUDA,    R_SOUDESU,
(1<<5) + 1,	_NA,		L_FUUDA,    R_SOUNA2,
(2<<5) + 1,	_RA,		L_FUUDA,    R_KATEI1,
(1<<5) + 1,	_NI,		L_FUUDA,    R_FUUNI,
0xff
};

static	Uchar	fzk_he[] = {
(0<<5)+0,			L_HE,       R_HE,
0xff
};

static	Uchar	fzk_be[] = {
(0<<5) + 2,	_KA,_RA,	L_BESI,     R_BEKARA,
(0<<5) + 1,	_KI,		L_BESI,     R_BEKI,
(0<<5) + 1,	_KU,		L_BESI,     R_BEKU,
(0<<5) + 1,	_SI,		L_BESI,     R_BESI,
0xff
};

static	Uchar	fzk_ho[] = {
(0<<5) + 1,	_U,		L_HOU,      R_HOU,
(0<<5) + 1,	_KA,		L_HOKA,     R_HOKA,
(0<<5) + 1,	_DO,		L_HODO,     R_HODO,
0xff
};

static	Uchar	fzk_ma[] = {
(0<<5) + 1,	_I,		L_MAI,      R_MAI,
(0<<5) + 1,	_SI,		L_MASU,     R_DESHI,
(1<<5) + 0,			L_MASU,	    R_MEIREI2,
(1<<5) + 1,	_XYO,		L_MASU,     R_SHO,
(2<<5) + 1,	_U,		L_MASU,     R_DESHOU,
(0<<5) + 1,	_SU,		L_MASU,     R_MASU,
(1<<5) + 1,	_RE,		L_MASURE,   R_KATEI2,
(0<<5) + 1,	_SE,		L_MASE,     R_MEIREI1,
(1<<5) + 1,	_NU,		L_MASU,     R_NU,
(1<<5) + 1,	_NN,		L_MASU,     R_NN2,
(0<<5) + 1,	_DE,		L_MADE,     R_MADE,
(0<<5) + 1,	_MA,		L_MAMA,     R_MAMA,
0xff
};

static	Uchar	fzk_mi[] = {
(0<<5) + 0,			L_YOUGEN,   R_MI,
(0<<5) + 2,	_TA,_I,		L_MITAIDA,  R_MITAI,
(2<<5) + 1,	_DA,		L_MITAIDA,  R_MITAIDA,
(3<<5) + 1,	_XTU,		L_MITAIDA,  R_ONBIN1,
(3<<5) + 2,	_RO,_U,		L_MITAIDA,  R_DAROU,
(2<<5) + 1,	_DE,		L_MITAIDA,  R_SOUDE,
(3<<5) + 1,	_SI,		L_MITAIDA,  R_DESHI,
(4<<5) + 1,	_XYO,		L_MITAIDA,  R_SHO,
(5<<5) + 1,	_U,		L_MITAIDA,  R_DESHOU,
(3<<5) + 1,	_SU,		L_MITAIDA,  R_SOUDESU,
(2<<5) + 1,	_NA,		L_MITAIDA,  R_SOUNA2,
(3<<5) + 1,	_RA,		L_MITAIDA,  R_KATEI1,
(2<<5) + 1,	_NI,		L_MITAIDA,  R_FUUNI,
(0<<5) + 1,	_YO,		L_YOUGEN,   R_MEIREI2,
(0<<5) + 1,	_RU,		L_YOUGEN,   R_SHUUSI,
(0<<5) + 1,	_RE,		L_YOUGEN,   R_KATEI2,
(0<<5) + 1,	_RO,		L_YOUGEN,   R_MEIREI1,
0xff
};

static	Uchar	fzk_mo[] = {
(0<<5) + 0,			L_MO1,      R_MO1,
(0<<5) + 0,			L_MO2,	    R_MO2,
(0<<5) + 2,	_XTU,_TE,	L_MOTTE,    R_MOTTE,
(0<<5) + 1,	_NO,		L_MONO,     R_MONO,
(0<<5) + 2,	_RA,_I,		L_YOUGEN,   R_IKI,
(1<<5) + 1,	_U,		L_YOUGEN,   R_SHUUSI,
(1<<5) + 1,	_E,		L_YOUGEN,   R_IKE,
(1<<5) + 2,	_O,_U,		L_YOUGEN,   R_AROU,
(1<<5) + 1,	_XTU,		L_YOUGEN,   R_ONBIN2,
(1<<5) + 1,	_WA,		L_YOUGEN,   R_MIZEN5,
(0<<5) + 1,	_NN,		L_WAYO,	    R_SHUUJO,
(1<<5) + 1,	_KA,		L_WAYO,	    R_SHUUJO,
0xff
};

static	Uchar	fzk_ya[] = {
(0<<5) + 0,			L_YA1,      R_YA1,
(0<<5) + 0,			L_YA2,      R_YA2,
(0<<5) + 2,	_SU,_I,		L_YASUI,    R_KSHUUSI,
(1<<5) + 2,	_KA,_XTU,	L_YASUI,    R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_YASUI,    R_KAROU,
(1<<5) + 1,	_KU,		L_YASUI,    R_TAKU,
(1<<5) + 2,	_KE,_RE,	L_YASUI,    R_KATEI3,
(1<<5) + 1,	_SA,		L_YASUI,    R_FZKMEISI,
(0<<5) + 1,	_XTU,		L_YOUGEN,   R_ONBIN2,
(0<<5) + 1,	_RA,		L_YARA,     R_YARA,
(1<<5) + 0,			L_YOUGEN,   R_MIZEN5,
(0<<5) + 1,	_RI,		L_YOUGEN,   R_IKI,
(0<<5) + 1,	_RU,		L_YOUGEN,   R_SHUUSI,
(0<<5) + 1,	_RE,		L_YOUGEN,   R_KATEI2,
(0<<5) + 2,	_RO,_U,		L_YOUGEN,   R_AROU,
0xff
};

static	Uchar	fzk_yu[] = {
(0<<5) + 1,	_E,		L_YUE,      R_YUE,
0xff
};

static	Uchar	fzk_yo[] = {
(0<<5) + 0,			L_YO,	    R_SHUUJO,
(0<<5) + 1,	_I,		L_YOI,      R_KSHUUSI,
(0<<5) + 1,	_U,		L_YOU,      R_YOU1,
(1<<5) + 0,			L_YOUDA,    R_YOU2,
(1<<5) + 1,	_DA,		L_YOUDA,    R_SOUDA,
(2<<5) + 1,	_XTU,		L_YOUDA,    R_ONBIN1,
(2<<5) + 2,	_RO,_U,		L_YOUDA,    R_DAROU,
(1<<5) + 1,	_DE,		L_YOUDA,    R_SOUDE,
(2<<5) + 1,	_SI,		L_YOUDA,    R_DESHI,
(3<<5) + 1,	_XYO,		L_YOUDA,    R_SHO,
(4<<5) + 1,	_U,		L_YOUDA,    R_DESHOU,
(2<<5) + 1,	_SU,		L_YOUDA,    R_SOUDESU,
(1<<5) + 1,	_NA,		L_YOUDA,    R_SOUNA2,
(2<<5) + 1,	_RA,		L_YOUDA,    R_KATEI1,
(1<<5) + 1,	_NI,		L_YOUDA,    R_FUUNI,
(0<<5) + 2,	_KA,_XTU,	L_YOI,      R_ONBIN1,
(1<<5) + 2,	_RO,_U,		L_YOI,      R_KAROU,
(0<<5) + 1,	_KU,		L_YOI,      R_TAKU,
(0<<5) + 2,	_KE,_RE,	L_YOI,      R_KATEI3,
(0<<5) + 1,	_SA,		L_YOI,	    R_FZKMEISI,
(0<<5) + 2,	_XTU,_TE,	L_YORU,     R_YOTTE,
(0<<5) + 2,	_NA,_XA,	L_YO,	    R_SHUUJO,
(1<<5) + 1,	_A,		L_YO,	    R_SHUUJO,
(0<<5) + 1,	_NE,		L_YO,	    R_SHUUJO,
(0<<5) + 1,	_RA,		L_YORU,     R_MIZEN4,
(0<<5) + 1,	_RI,		L_YORI,     R_YORI1,
(1<<5) + 0,			L_YORU,	    R_YORI2,
(0<<5) + 1,	_RU,		L_YORU,	    R_SHUUSI,
(0<<5) + 1,	_RE,		L_YORU,     R_KATEI3,
0xff
};

static	Uchar	fzk_ra[] = {
(0<<5) + 2,	_SI,_I,		L_RASII,    R_RASII,
(1<<5) + 2,	_KA,_XTU,	L_RASII,    R_ONBIN1,
(1<<5) + 1,	_KI,		L_RASII,    R_GOTOKI,
(1<<5) + 1,	_KU,		L_RASII,    R_TAKU,
(1<<5) + 2,	_XYU,_U,	L_RASII,    R_RASHUU,
(0<<5) + 1,	_RE,		L_RARERU,   R_RARE,
(1<<5) + 1,	_YO,		L_RARERU,   R_MEIREI2,
(1<<5) + 1,	_RU,		L_RARERU,   R_SHUUSI,
(1<<5) + 1,	_RE,		L_RARERU,   R_KATEI2,
(1<<5) + 1,	_RO,		L_RARERU,   R_MEIREI1,
0xff
};

static	Uchar	fzk_re[] = {
(0<<5) + 0,			L_RERU,	    R_RARE,
(0<<5) + 1,	_YO,		L_RERU,     R_MEIREI2,
(0<<5) + 1,	_RU,		L_RERU,     R_SHUUSI,
(0<<5) + 1,	_RE,		L_RERU,	    R_KATEI2,
(0<<5) + 1,	_RO,		L_RERU,	    R_MEIREI1,
0xff
};

static	Uchar	fzk_wa[] = {
(0<<5) + 0,			L_WA,	    R_SHUUJO,
(0<<5) + 1,	_KE,		L_WAKE,     R_WAKE,
(0<<5) + 1,	_NE,		L_WANE,     R_SHUUJO,
(0<<5) + 1,	_YO,		L_WAYO,     R_SHUUJO,
0xff
};

static	Uchar	fzk_nn[] = {
(0<<5) + 0,			L_NN1,      R_NN1,
(0<<5) + 0,			L_NN2,      R_NN2,
0xff
};



Uchar	*fzkadr[] = {
	0,	fzk_a,	0,	fzk_i,	0,
	fzk_u,	0,	fzk_e,	0,	fzk_o,
	fzk_ka,	fzk_ga,	fzk_ki,	0,	fzk_ku,
	fzk_gu,	fzk_ke,	0,	fzk_ko,	fzk_go,
	fzk_sa,	fzk_za,	fzk_si,	fzk_ji,	fzk_su,
	fzk_zu,	fzk_se,	fzk_ze,	fzk_so,	fzk_zo,
	fzk_ta,	fzk_da,	fzk_ti,	fzk_di,	fzk_xtu,
	fzk_tu,	0,	fzk_te,	fzk_de,	fzk_to,
	fzk_do,	fzk_na,	fzk_ni,	fzk_nu,	fzk_ne,
	fzk_no,	fzk_ha,	fzk_ba,	0,	0,
	0,	0,	fzk_hu,	0,	0,
	fzk_he, fzk_be,	0,	fzk_ho,	0,
	0, 	fzk_ma,	fzk_mi,	0,	0,
	fzk_mo, 0,	fzk_ya,	0,	fzk_yu,
	0, 	fzk_yo,	fzk_ra,	0,	0,
	fzk_re, 0,	0,	fzk_wa,	0,
	0, 	fzk_wo,	fzk_nn
};
