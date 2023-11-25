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
 * $SonyRCSfile: ph2knj.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:02:09 $
 */





#include "sj_kcnv.h"
#include "sj_yomi.h"



Int	sj2cd_chr(), sstrlen();
Void	mkjiritu(), mkbunsetu(), wakachi(), mk2claus();
Void	freework();
Void    selclrec();
CLREC	*free_clst();
JREC	*free_jlst();



Int	ph2knj(zyomi, kanji, knjlen)
Uchar	*zyomi;		
Uchar	*kanji;		
Int	knjlen;		
{
	Uchar	*ptr;
	Uchar	*dst;
	Int	i;

	
	for (ptr = zyomi, dst = hyomi, i = 0 ; *ptr ; ) {
		if (i++ >= MaxPhInputLen) { *kanji = 0; return 0; }
		ptr += sj2cd_chr(ptr, dst++);
	}
	*dst = 0;

	
	freework();
	prevclgrm = 0;
	prevclrow = 0;

	khcount = 0;				
	nextcllen = 0;				

	
	inputyomi = zyomi;
	cnvstart = ystart = hyomi;
	cnvlen = sstrlen(hyomi);

	
	kanjipos = kanji;
	kanjilen = knjlen;

	
	while (cnvlen && kanjilen) {
		
		if (!clt1st) {
			
			mkjiritu(DO_CLSTUDY | DO_IKKATU);

			
			mkbunsetu();

			
			if (!maxclptr) wakachi();

			jrt1st = maxjptr;
			clt1st = maxclptr;
		}

		
		mk2claus();

		
		selclrec();

		
		prevclgrm = selcl -> jnode -> hinsi;
		prevclrow = selcl -> right;

		
                clt1st = free_clst(clt1st, (Int)selcl -> cllen);
                jrt1st = free_jlst(jrt1st);

		
		cnvstart += selcl -> cllen;
		cnvlen -= selcl -> cllen;

                cvtphknj();
 
                clt1st = clt2nd;
                jrt1st = jrt2nd;
	}

        if (!kanjilen) {
                free_clall(clt1st);
                free_jall (jrt1st);
        }

	*kanjipos = 0;

	return(inputyomi - zyomi);
}


