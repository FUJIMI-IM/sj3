/*
 * Copyright (c) 2004  Hiroo Ono <hiroo at oikumene.gcd.org>
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
 * $Id: dicttool.h,v 1.1 2004/07/01 17:11:27 hiroo Exp $
 */

#ifndef _DICTTOOL_H_
#define _DICTTOOL_H_  1

#include <stdio.h>
#include <sys/types.h>
#include "sj_struct2.h"

/* char.c */
int cnvyomi (int code);
int h2kcode (int code);
int codesize (unsigned char code);
void output_knj (FILE* fp, unsigned char* p, int l);
void output_str (FILE* fp, char* p);
void output_int (FILE* fp, int* p);
void output_yomi (FILE* fp, unsigned char* p);

/* cnvhinsi.c */
int cnvhinsi (unsigned char *buf);

/* file.c */
FILE* Fopen (char* filename, char* type);
void Fclose (FILE* fp);
int Fseek (FILE* fp, long ofs, int ptr);
long Ftell (FILE* fp);
off_t Fsize (char* filename);
int Fread (char* p, int s, int n, FILE* fp);
int Fwrite (char* p, int s, int n, FILE* fp);
int Fgetc (FILE* fp);
void Fflush (FILE* fp);

/* hindo.c */
int check_hindo (unsigned char* ptr, int len);
void knjhnd_set (unsigned char* p, int l);
void knjhnd_reset (unsigned char* p, int l);
int decide_knjask (void);
void clear_hindo (void);
void clear_assyuku (void);

/* knjcvt.c */
unsigned char* knjofscvt (unsigned char* ptr, int len, int* ret);
unsigned char* knjcvt (unsigned char* ptr, int len, int* ret);

/* makedict.c */
void parse (int argc, char** argv);
void usage (char* name);
int getch (void);
void mark_file (FILE* out);

/* makelist.c */
void clear_list (void);
void flush_douon (void);
void makelist (int* yomi, int* kanji, int hinsi, int*  atr);

/* makeseg.c */
size_t make_knjstr (unsigned char* src, int len, unsigned char* dst);
void makeseg (void);
void makehead (unsigned char* dict_name);

/* memory.c */
void* Malloc (size_t size);
void* Zalloc (size_t size);
void Free (void *p);

/* offset.c */
void set_ofsrec (unsigned char* ptr, int len, int ofs);
int  isknjexist (unsigned char* knj, int len);
void clear_ofsrec (void);
OffsetRec* real_ofsrec (unsigned char* ptr);

/* readline.c */
int* readline (void);
void setline (void (*func)(int*, int*, int, int*));

/* string.c */
int bubun_str (unsigned char* p1, int l1, unsigned char* p2, int l2);
int overlap_str (unsigned char *p1, int l1, unsigned char* p2, int l2);
int top_strcmp (int* src, int* dst);
int last_strcmp (int* src, int* dst);
int string_cmp (unsigned char* p1, int l1, unsigned char* p2, int l2);

#endif /* _DICTTOOL_H_ */

