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
 * $SonyRCSfile: sj_yomi.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:24 $
 */





#ifndef	_SJ_YOMICODE

#define	_SJ_YOMICODE

#define _TYOUON		0x01	
#define _IGETA		0x02	
#define _ATTO		0x03	
#define _YUUBIN		0x04	

#define	N_0		0x10	
#define	N_1		0x11	
#define	N_2		0x12	
#define	N_3		0x13	
#define	N_4		0x14	
#define	N_5		0x15	
#define	N_6		0x16	
#define	N_7		0x17	
#define	N_8		0x18	
#define	N_9		0x19	

#define	A_A		0x1A	
#define	A_B		0x1B	
#define	A_C		0x1C	
#define	A_D		0x1D	
#define	A_E		0x1E	
#define	A_F		0x1F	
#define	A_G		0x20	
#define	A_H		0x21	
#define	A_I		0x22	
#define	A_J		0x23	
#define	A_K		0x24	
#define	A_L		0x25	
#define	A_M		0x26	
#define	A_N		0x27	
#define	A_O		0x28	
#define	A_P		0x29	
#define	A_Q		0x2A	
#define	A_R		0x2B	
#define	A_S		0x2C	
#define	A_T		0x2D	
#define	A_U		0x2E	
#define	A_V		0x2F	
#define	A_W		0x30	
#define	A_X		0x31	
#define	A_Y		0x32	
#define	A_Z		0x33	
	
#define	A_a		0x34	
#define	A_b		0x35	
#define	A_c		0x36	
#define	A_d		0x37	
#define	A_e		0x38	
#define	A_f		0x39	
#define	A_g		0x3A	
#define	A_h		0x3B	
#define	A_i		0x3C	
#define	A_j		0x3D	
#define	A_k		0x3E	
#define	A_l		0x3F	
#define	A_m		0x40	
#define	A_n		0x41	
#define	A_o		0x42	
#define	A_p		0x43	
#define	A_q		0x44	
#define	A_r		0x45	
#define	A_s		0x46	
#define	A_t		0x47	
#define	A_u		0x48	
#define	A_v		0x49	
#define	A_w		0x4A	
#define	A_x		0x4B	
#define	A_y		0x4C	
#define	A_z		0x4D	

#define	_XA		0x4E	
#define	_A		0x4F	
#define	_XI		0x50	
#define	_I		0x51	
#define	_XU		0x52	
#define	_U		0x53	
#define	_XE		0x54	
#define	_E		0x55	
#define	_XO		0x56	
#define	_O		0x57	
#define	_KA		0x58	
#define	_GA		0x59	
#define	_KI		0x5A	
#define	_GI		0x5B	
#define	_KU		0x5C	
#define	_GU		0x5D	
#define	_KE		0x5E	
#define	_GE		0x5F	
#define	_KO		0x60	
#define	_GO		0x61	
#define	_SA		0x62	
#define	_ZA		0x63	
#define	_SI		0x64	
#define	_ZI		0x65	
#define	_SU		0x66	
#define	_ZU		0x67	
#define	_SE		0x68	
#define	_ZE		0x69	
#define	_SO		0x6A	
#define	_ZO		0x6B	
#define	_TA		0x6C	
#define	_DA		0x6D	
#define	_TI		0x6E	
#define	_DI		0x6F	
#define	_XTU		0x70	
#define	_TU		0x71	
#define	_DU		0x72	
#define	_TE		0x73	
#define	_DE		0x74	
#define	_TO		0x75	
#define	_DO		0x76	
#define	_NA		0x77	
#define	_NI		0x78	
#define	_NU		0x79	
#define	_NE		0x7A	
#define	_NO		0x7B	
#define	_HA		0x7C	
#define	_BA		0x7D	
#define	_PA		0x7E	
#define	_HI		0x7F	
#define	_BI		0x80	
#define	_PI		0x81	
#define	_HU		0x82	
#define	_BU		0x83	
#define	_PU		0x84	
#define	_HE		0x85	
#define	_BE		0x86	
#define	_PE		0x87	
#define	_HO		0x88	
#define	_BO		0x89	
#define	_PO		0x8A	
#define	_MA		0x8B	
#define	_MI		0x8C	
#define	_MU		0x8D	
#define	_ME		0x8E	
#define	_MO		0x8F	
#define	_XYA		0x90	
#define	_YA		0x91	
#define	_XYU		0x92	
#define	_YU		0x93	
#define	_XYO		0x94	
#define	_YO		0x95	
#define	_RA		0x96	
#define	_RI		0x97	
#define	_RU		0x98	
#define	_RE		0x99	
#define	_RO		0x9A	
#define	_XWA		0x9B	
#define	_WA		0x9C	
#define	_WI		0x9D	
#define	_WE		0x9E	
#define	_WO		0x9F	
#define	_NN		0xA0	
#define	_VU		0xA1	
#define	_XKA		0xA2	
#define	_XKE		0xA3	

#define	Y_KUTEN		0xF2	
#define	Y_TOUTEN	0xF3	
#define	Y_COMMA		0xF4	
#define	Y_PERIOD	0xF5	
#define	Y_QUOTA		0xF6	
#define	Y_BIKKURI	0xF7	
#define	Y_S_KIGOU1	0xF8	
#define	Y_S_KIGOU2	0xF9	
#define	Y_L_KAKKO	0xFA	
#define	Y_R_KAKKO	0xFB	
#define	Y_KIGOU		0xFC	
#define	Y_HANKAKU	0xFD	
#define	Y_ZENKAKU	0xFE	

#define	_NIL		0

#endif
