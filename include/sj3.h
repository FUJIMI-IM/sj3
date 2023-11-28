/*
 * Copyright (c) 2004  Hiroo Ono
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
 * $Id: sj3.h,v 1.1 2004/07/02 18:31:25 hiroo Exp $
 */

#ifndef _SJ3_H_
#define _SJ3_H_  1

/* funckey.c */
void mk_cvtkey(char *term);
char *getkey2(char *istr, char *ostr);
void clear_ukeys(void);
void set_func(struct wordent word[]);
wchar16_t eval_keys(wchar16_t *s, wchar16_t *p);
void clear_key(void);
void clear_commkeys(void);
int eval_key(wchar16_t *s);
void clear_etckeys(void);
int set_etckeys(struct wordent word[]);
wchar16_t parse_command(wchar16_t *s, int *count);

/* sj3.c */
int main(int argc, char **argv);
void makecore(void);
void init(char **argv);
void init_env(void);
void parsearg(int argc, char **argv);
void usage(void);
void sjinit(void);
void setshellname(void);
void getfixtty(void);
void get_ttymode(void);
void getmaster(void);
#ifdef __sony_news
void set_jmode(int mode);
#else
void set_jmode(void);
#endif
void fixtty(void);
void setdev(void);
void forkshell(void);
void sj3_setenv(char *ename, char *eval, char *buf);
void shellprocess(void);
void getslave(void);
void execcmd(char *cmd, char **ap);
void fail(void);
void done(void);
void done2(void);
void done3(void);
void exitprocess(void);
void suspend(void);
void clearutmpentry(void);
void onwinch(void);
void SetRegion(void);
void set_tty_size(void);
void setsjserv(char *hname);
char *chmyhname(char *hname);

#endif /* _SJ3_H_ */

