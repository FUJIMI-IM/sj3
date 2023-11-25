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
 * $SonyRCSfile: functbl.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:46 $
 */




#include "sj_rename.h"
#include "sj_typedef.h"



Void	num_type00(), num_type01(), num_type02(), num_type03();
Void	num_type04(), num_type05(), num_type06(), num_type07();
Void	num_type08(), num_type09(), num_type10(), num_type11();
Void	num_type12(), num_type13(), num_type14();
VFunc	cvtnum_func[] = {
	num_type00,	num_type01,	num_type02,	num_type03,
	num_type04,	num_type05,	num_type06,	num_type07,
	num_type08,	num_type09,	num_type10,	num_type11,
	num_type12,	num_type13,	num_type14
};



Uchar	*getkan_hira(), *getkan_kata(), *getkan_knj(), *getkan_ofs();
Uchar	*getkan_norm(), *getkan_none();
UCPFunc	getkan_func[] = {
	getkan_norm,	getkan_norm,	getkan_hira,	getkan_kata,
	getkan_knj,	getkan_ofs,	getkan_norm,	getkan_none,
	getkan_norm,	getkan_norm,	getkan_hira,	getkan_kata,
	getkan_knj,	getkan_ofs,	getkan_norm,	getkan_none
};



Int	setj_norm1(), setj_norm2(), setj_knj(), setj_ofs(), setj_atrb();
IFunc	setjrec_func[] = {
	setj_norm2,	setj_norm2,	setj_norm1,	setj_norm1,
	setj_knj,	setj_ofs,	setj_norm2,	setj_atrb,
	setj_norm2,	setj_norm2,	setj_norm1,	setj_norm1,
	setj_knj,	setj_ofs,	setj_norm2,	setj_atrb
};



Int	hiraknj_atrb(), hiraknj_ofs(), hiraknj_knj(), hiraknj_hask();
Int	hiraknj_kask(), hiraknj_norm(), hiraknj_hira();
IFunc	hiraknj_func[] = {
	hiraknj_hira,	hiraknj_norm,	hiraknj_hask,	hiraknj_kask,
	hiraknj_knj,	hiraknj_ofs,	hiraknj_norm,	hiraknj_atrb,
	hiraknj_hira,	hiraknj_norm,	hiraknj_hask,	hiraknj_kask,
	hiraknj_knj,	hiraknj_ofs,	hiraknj_norm,	hiraknj_atrb
};



Uchar	*makekan_1byte(), *makekan_knj(), *makekan_ofs();
Uchar	*makekan_norm(), *makekan_none();
UCPFunc	makekan_func[] = {
	makekan_norm,	makekan_norm,	makekan_1byte,	makekan_1byte,
	makekan_knj,	makekan_ofs,	makekan_norm,	makekan_none,
	makekan_norm,	makekan_norm,	makekan_1byte,	makekan_1byte,
	makekan_knj,	makekan_ofs,	makekan_norm,	makekan_none
};
