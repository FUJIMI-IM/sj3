/*-
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2023  Masanori Ogino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SJ3_SJ_STRING_H_
#define SJ3_SJ_STRING_H_

int sj3_str_sjistoeuc(unsigned char *, int, unsigned char *, unsigned char *, int *);
int sj3_str_euctosjis(unsigned char *, int, unsigned char *, unsigned char *, int *);

int sj3_sjistoeuclen(unsigned char *, int);

int sj3_sjistoeuc(unsigned char *, int, unsigned char *, int);
int sj3_euctosjis(unsigned char *, int, unsigned char *, int);

void sj_euc2sjis (unsigned char *);
void sj_jis2sjis (unsigned char *);
void sj_sjis2euc (unsigned char *);
void sj_sjis2jis (unsigned char *);

unsigned short sj3_jis2sjis(unsigned short);
unsigned short sj3_jis2euc(unsigned short);
unsigned short sj3_sjis2jis(unsigned short);
unsigned short sj3_euc2sjis(unsigned short);
unsigned short sj3_sjis2euc(unsigned short);

#endif
