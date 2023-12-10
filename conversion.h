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
 * $SonyRCSfile: conversion.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:43 $
 */



#include "sj2.h"
#include "sj3libsj3.h"
#include "henkan.h"

typedef struct bunsetu_sj3	Bunsetu;

typedef struct _Conversion {
	unsigned short	out_mode[BUFFLENGTH*2];

	unsigned short	Plen[BUFFLENGTH];
	unsigned short	OrgPlen[BUFFLENGTH];
	unsigned short	Bpoint[BUFFLENGTH];
	unsigned short	Edit[BUFFLENGTH];
	unsigned short	span_l[BUFFLENGTH];
	unsigned short	Elen[BUFFLENGTH];
	unsigned short	Displen[BUFFLENGTH];

	unsigned short Vindex[BUFFLENGTH * 2];

	short	ha_point;		
	short	ha_epoint;		
	short	out_point;
	short	out_epoint;
	short	span_point;		
	short	Bnum;			
	short	Enum;			
	short	e_point;		

	unsigned short line;
	unsigned short column;

	unsigned short	PushCurrentVcol;
	unsigned short	Vlen;
	unsigned short	SavedRow;
	unsigned short	SavedCol;
	unsigned short	CurrentVcol;
	unsigned short	MaxVcol;

	short	CurBun;			
	short	PreBun;			

	Bunsetu	Bun[BUFFLENGTH];	

	wchar16_t	out_buf[BUFFLENGTH * 2];
	unsigned char	in_mode[BUFFLENGTH * 2];
	wchar16_t	*span_p[BUFFLENGTH];	
	wchar16_t	*Ppoint[BUFFLENGTH];	
	wchar16_t	*Epoint[BUFFLENGTH];	
	wchar16_t	Halpha[BUFFLENGTH];	
	unsigned char	kettei[BUFFLENGTH];
	unsigned char	Pkettei[BUFFLENGTH];
	unsigned char	Pedited[BUFFLENGTH];
	wchar16_t	Ebuff[EDITLEN];		
	unsigned char	Bkettei[BUFFLENGTH];
	unsigned char	Dispmod[BUFFLENGTH];
	wchar16_t	yomiout[BUFFLENGTH * 2];
	wchar16_t	kanjiout[BUFFLENGTH * 4];

	unsigned char	Imode;
	unsigned char	BuffMode;
	unsigned char	Kanji;
	unsigned char	Dflag;
	unsigned char	word_regist;
	unsigned char	Cflag;
	unsigned char	henkanflag;

	unsigned char	wrap;
} Conversion;


wchar16_t osave_buf[BUFFLENGTH * 2];  
unsigned short osave_mode[BUFFLENGTH * 2];
unsigned short isave_mode[BUFFLENGTH * 2];
unsigned short os_point;

wchar16_t hsave_buf[BUFFLENGTH];
unsigned short hs_point;
