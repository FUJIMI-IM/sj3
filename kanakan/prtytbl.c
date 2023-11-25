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
 * $SonyRCSfile: prtytbl.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:11 $
 */



#include "sj_rename.h"
#include "sj_typedef.h"

Uchar	taipri[7][45] = {

	{ 0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  3,  3,  3,  3,  3,  3,  0,  0,
		  0,  3, 20,  4,  3,  3,  4,  4,  4,  4,
	 	  4,  4,  4, 20,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},

	{ 0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  3,  3,  3,  3,  3,  3,  0,  0,
		  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	 	  4,  4,  4, 20,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},

	{ 0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  3,  3,  3,  3,  3,  3,  0,  0,
		  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
	 	  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  6},

	{ 0,  5,  5,  5,  5,  5,  5,  5,  5,  5,
		  5,  5,  3,  3,  3,  3,  3,  3,  0,  0,
		  0,  3,  3,  3,  4,  5,  4,  4,  4,  4,
	 	  4,  4,  4,  4,  4,  4,  6,  6,  6,  8,  4,  4,  4,  4,  6},

	{ 0,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		  7,  7,  4,  4,  4,  4,  4,  4,  0,  0,
		  0,  4,  4,  4, 20, 20,  4,  4,  4,  4,
	 	  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  6},

	{ 0,  8,  8,  8,  8,  8,  7,  8,  8,  8,
		  8,  8,  3,  6,  7,  3,  3,  5,  0,  0,
		  0,  3,  3,  3,  3,  3,  5,  5,  5,  5,
	 	  5,  5,  5,  5,  5,  5,  8,  8, 11,  5,  7,  7,  7,  7,  7},

	{ 0,  5,  6,  6,  6,  6,  5,  6,  6,  6,
		  6,  6,  3,  3,  3,  3,  3,  5,  0,  0,
		  0,  3,  3,  3,  3,  3,  5,  5,  5,  5,
 		  4,  5,  5,  5,  5,  5,  5,  6,  6,  5,  5,  5,  5,  5,  5}
	};



Uchar	sttpri[5][31] = {

	{ 0,  7,  7,  7,  7,  7,  7,  7,  7,  7,
		  7,  7,  2,  4,  4,  4,  4,  4,  0,  0,
	 	  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, },

	{ 4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  4,  4,  4,  4,  4,  4,  0,  0,
	 	  0,  4,  4,  4, 10, 10,  4,  4,  4,  4,  4, },

	{ 4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  4,  4,  4,  4,  4,  4,  0,  0,
	 	  0, 10, 10,  4,  4,  4,  4,  4,  4,  4,  4, },

	{ 0,  4,  4,  4,  4,  4,  4,  4,  4,  4,
		  4,  4,  4,  4,  4,  4,  4,  4,  0,  0,
	 	  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  15, },

	{ 0, 12, 12, 12, 12, 12, 12, 12, 12, 12,
		 12, 12,  9,  9,  9,  9,  9,  9,  0,  0,
	 	  0,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4, }
	};
