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
 * $SonyRCSfile: wchar16.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:52 $
 */



#ifndef _WCHARH
#define _WCHARH


#if defined(__sony_news) && defined(SVR4)
#define _WCHAR_T
typedef unsigned short  wchar_t;
#include <stdlib.h>
#include <ctype.h>
#include <widec.h>
#include <wctype.h>
typedef wchar_t  wchar16_t;
#else
#include <stdlib.h>
#include <ctype.h>
typedef unsigned short  wchar16_t;
#endif
#include "key.h"

#ifndef SS2
#define SS2 0x8e
#define SS3 0x8f
#endif

#if !defined(__sony_news) || (defined(__sony_news) && !defined(SVR4))

#ifdef iswcntrl
#undef iswcntrl
#endif
#define iswcntrl(wc)         sj3_iswcntrl16(wc)
#ifdef iswupper
#undef iswupper
#endif
#define iswupper(wc)         sj3_iswupper16(wc)
#ifdef iswdigit
#undef iswdigit
#endif
#define iswdigit(wc)         sj3_iswdigit16(wc)
#ifdef iswxdigit
#undef iswxdigit
#endif
#define iswxdigit(wc)        sj3_iswxdigit16(wc)
#ifdef wslen
#undef wslen
#endif
#define wslen(ws)            sj3_wslen16(ws)
#ifdef wscmp
#undef wscmp
#endif
#define wscmp(ws1, ws2)      sj3_wscmp16(ws1, ws2)
#ifdef wsncmp
#undef wsncmp
#endif
#define wsncmp(ws1, ws2, n)  sj3_wsncmp16(ws1, ws2, n)
#ifdef wscpy 
#undef wscpy
#endif
#define wscpy(ws1, ws2)      sj3_wscpy16(ws1, ws2)
#ifdef wsncpy
#undef wsncpy
#endif
#define wsncpy(ws1, ws2, n)  sj3_wsncpy16(ws1, ws2, n)
#ifdef wscat
#undef wscat
#endif
#define wscat(ws1, ws2)      sj3_wscat16(ws1, ws2)
#ifdef mbstowcs
#undef mbstowcs
#endif
#define mbstowcs(mb, ws, n)  sj3_mbstowcs16(mb, ws, n)
#ifdef wcstombs
#undef wcstombs
#endif
#define wcstombs(ws, mb, n)  sj3_wcstombs16(ws, mb, n)
#endif


#define LC_CTYPE_SHIFTJIS 1
#define LC_CTYPE_EUC 2

#define IsEUCHojo(x) (((unsigned char) (x)) == SS3)




#define WcX0208MASK 0xffff
#define WcX0212MASK 0xff7f
#define WcJISMASK 0x7f7f
#define WcLowMASK 0x00ff
#define WcHighMASK 0xff00

#define WcASCIIFLAG 0x0000
#define WcX0208FLAG 0x8080
#define WcX0212FLAG 0x8000
#define WcX0201FLAG 0x0080

#define WcLowByte(w)  ((w) & 0xff)
#define WcHighByte(w) (((w) >> 8) & 0xff)

#define WcIsASCII(w)  ((((wchar16_t) (w)) & WcX0208FLAG) == WcASCIIFLAG)
#define WcIsX0208(w)  ((((wchar16_t) (w)) & WcX0208FLAG) == WcX0208FLAG)
#define WcIsX0201(w)  ((((wchar16_t) (w)) & WcX0208FLAG) == WcX0201FLAG)
#define WcIsX0212(w)  ((((wchar16_t) (w)) & WcX0208FLAG) == WcX0212FLAG)

#define WcIsZENKAKU(w) ((((wchar16_t) (w)) & WcX0212FLAG) == WcX0212FLAG)
#define WcIsHANKAKU(w) ((((wchar16_t) (w)) & WcX0212FLAG) == WcASCIIFLAG)
#define WcIsKANJI(w)  WcIsZENKAKU(w)
#define WcIsKANA(w)  WcIsX0201(w)

#define WcSetX0212(c1, c2) ((((c1) & 0xff) << 8) + ((c2) & 0x7f))
#define WcJIS2X0208(w)  ((w) | WcX0208FLAG)
#define WcJIS2X0212(w)  ((w) | WcX0212FLAG)
#define Wc2JIS(w)  ((w) & WcJISMASK)


#define WCSJrun		WcMessages[0]

#define WCMode_buff	WcMessages[1]
#define WCMode_unbuff	WcMessages[2]

#define WCMode_Zhira	WcMessages[3]
#define WCMode_Zkata	WcMessages[4]
#define WCMode_Hkata	WcMessages[5]
#define WCMode_Zalpha	WcMessages[6]
#define WCMode_Halpha	WcMessages[7]

#define WCGmode           WcMessages[8]
#define WCGEtc		WcMessages[9]
#define WCGetc		WcMessages[10]

#define WCGHelp		WcMessages[11]
#define WCMHelp		WcMessages[12]



#define WCGCshift	        WcMessages[13]
#define WCGCeuc		WcMessages[14]
#define WCGCeuc2		WcMessages[15]
#define WCGCjis		WcMessages[16]
#define WCGCjis2		WcMessages[17]
#define WCGCkuten	        WcMessages[18]
#define WCGCkuten2	WcMessages[19]



#define WCGtoroku	        WcMessages[20]
#define WCMhani		WcMessages[21]
#define WCMyomi		WcMessages[22]
#define WCTOUROKU_OK	WcMessages[23]
#define WCSYOUKYO_OK	WcMessages[24]
#define WCTOUROKU_HIN	WcMessages[25]
#define WCTOUROKU_SUCC	WcMessages[26]
#define WCYomi           WcMessages[27]
#define WCOne            WcMessages[28]
#define WCY              WcMessages[29]
#define WCExist          WcMessages[30]
#define WCYomiTouroku    WcMessages[31]
#define WCKanjiTouroku   WcMessages[32]
#define WCOnajiTouroku   WcMessages[33]
#define WCUserTouroku    WcMessages[34]
#define WCJishoLock      WcMessages[35]
#define WCSystemError    WcMessages[36]
#define WCKanma          WcMessages[37]


#define WCGsyoukyo	WcMessages[38]
#define WCSYOUKYO_SUCC	WcMessages[39]
#define WCNotExist       WcMessages[40]
#define WCUserLock       WcMessages[41]
#define WCYomiShokyo     WcMessages[42]
#define WCKanjiShokyo    WcMessages[43]
#define WCShokyoError    WcMessages[44]



#define WCSUCCsjrc	WcMessages[45]
#define WCFAILsjrc	WcMessages[46]
#define WCNOTHsjrc       WcMessages[47]


#define WCWeight_mes     WcMessages[48]



#define WCGrcon		WcMessages[49]
#define WCRserv		WcMessages[50]
#define WCRserv_OK	WcMessages[51]
#define WCDiscon		WcMessages[52]

#define WCSpace          WcMessages[53]
#define WCSpace2         WcMessages[54]
#define WCContinue       WcMessages[55]


#define WCServer_Dead	WcMessages[56]
#define WCConnected		WcMessages[57]
#define WCNconnect		WcMessages[58]
#define WCNopen_Mdic	WcMessages[59]
#define WCNmake_Udir	WcMessages[60]
#define WCNmake_Udic	WcMessages[61]
#define WCNopen_Udic	WcMessages[62]
#define WCNmake_Study	WcMessages[63]
#define WCNopen_Study	WcMessages[64]



#define WCGedit		WcMessages[65]


#define WCTooLong        WcMessages[67]
#define WCDollar         WcMessages[68]


#define WCOn             WcMessages[69]
#define WCOff            WcMessages[70]


#define WCMeishi         WcMessages[71]
#define WCMyouji         WcMessages[72]
#define WCNamae          WcMessages[73]
#define WCTimei          WcMessages[74]
#define WCSahen          WcMessages[75]


#define WCKara           WcMessages[76]
#define WCF1             WcMessages[77]
#define WCF2             WcMessages[78]
#define WCF3             WcMessages[79]
#define WCF4             WcMessages[80]
#define WCF5             WcMessages[81]
#define WCF6             WcMessages[82]
#define WCF7             WcMessages[83]
#define WCF8             WcMessages[84]
#define WCF9             WcMessages[85]
#define WCF10             WcMessages[86]
#define WCF11             WcMessages[87]
#define WCF12             WcMessages[88]
#define WCF13             WcMessages[89]
#define WCF14             WcMessages[90]
#define WCF15             WcMessages[91]
#define WCF16             WcMessages[92]
#define WCF17             WcMessages[93]
#define WCF18             WcMessages[94]
#define WCF19             WcMessages[95]
#define WCF20             WcMessages[96]
#define WCSelect          WcMessages[97]
#define WCKanji           WcMessages[98]
#define WCCancel          WcMessages[99]
#define WCExecute         WcMessages[100]
#define WCUp              WcMessages[101]
#define WCDown            WcMessages[102]
#define WCRight           WcMessages[103]
#define WCLeft            WcMessages[104]

#define WCStart           WcMessages[105]
#define WCHenkan          WcMessages[106]
#define WCMuhen           WcMessages[107]
#define WCKettei          WcMessages[108]
#define WCHira            WcMessages[109]
#define WCHalpha          WcMessages[110]
#define WCZalpha          WcMessages[111]
#define WCHkata           WcMessages[112]
#define WCZkata           WcMessages[113]
#define WCCode            WcMessages[114]
#define WCEtc             WcMessages[115]
#define WCRightl          WcMessages[116]
#define WCLeftl           WcMessages[117]
#define WCKaku            WcMessages[118]
#define WCSyuku           WcMessages[119]
#define WCEdit            WcMessages[120]
#define WCToggle          WcMessages[121]
#define WCWrapback        WcMessages[122]
#define WCWrap            WcMessages[123]
#define WCSjrc            WcMessages[124]
#define WCBuffer          WcMessages[125]
#define WCToroku          WcMessages[126]
#define WCSyoukyo         WcMessages[127]
#define WCHelp            WcMessages[128]
#define WCHaKettei        WcMessages[129]
#define WCFlush           WcMessages[130]
#define WCDebug           WcMessages[131]
#define WCCancell         WcMessages[132]
#define WCSaihenkan       WcMessages[133]
#define WCIgnore          WcMessages[134]
#define WCTop             WcMessages[135]
#define WCEnd             WcMessages[136]
#define WCReconnect       WcMessages[137]
#define WCKill            WcMessages[138]


#define WCKey             WcMessages[139]
#define WCEscape          WcMessages[140]
#define WCSjxkey          WcMessages[141]
#define WCEtckey          WcMessages[142]
#define WCIntr            WcMessages[143]
#define WCBskey           WcMessages[144]
#define WCDelkey          WcMessages[145]
#define WCSetnormal       WcMessages[146]
#define WCThroughnext     WcMessages[147]
#define WCInitialmode     WcMessages[148]
#define WCHelpmode        WcMessages[149]
#define WCDefaultcode     WcMessages[150]
#define WCMuhenkanmode    WcMessages[151]
#define WCMuhenkaninedit  WcMessages[152]
#define WCMuhenkantoggle  WcMessages[153]
#define WCGuide           WcMessages[154]
#define WCForkshell       WcMessages[155]
#define WCExecshell       WcMessages[156]
#define WCDictionary      WcMessages[157]
#define WCUserdic         WcMessages[158]
#define WCMaindic         WcMessages[159]
#define WCRomaji          WcMessages[160]
#define WCBstudy          WcMessages[161]
#define WCSilent          WcMessages[162]
#define WCFlushafterconversion WcMessages[163]
#define WCRkerrbell       WcMessages[164]
#define WCServer          WcMessages[165]

#define WCShiftjis        WcMessages[166]
#define WCSjis            WcMessages[167]
#define WCEuc             WcMessages[168]
#define WCEuc2            WcMessages[169]
#define WCJis             WcMessages[170]
#define WCJis2            WcMessages[171]
#define WCKuten           WcMessages[172]
#define WCKuten2          WcMessages[173]

#define WCBuff            WcMessages[174]
#define WCUnbuff          WcMessages[175]
#define WCDirect          WcMessages[176]

#define WCSj2             WcMessages[177]
#define WCShift           WcMessages[178]

#define WCFunction        WcMessages[179]

#define WCUnbuffer        WcMessages[180]
#define WCFuncetc         WcMessages[181]

extern wchar16_t *WcMessages[];



struct wcvtkey { 
        wchar16_t *key_word;
        wchar16_t key_seq[SEQLEN];
};


struct functbl {
        wchar16_t *keyword;
        int (*func)();
};

#endif 
