/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */


#ifndef _SERVER_H_
#define _SERVER_H_  1

#include "Struct.h"

/* comuni.c */
void socket_init(void);
void open_socket(void);
void close_socket(void);
void communicate(void);
void put_flush(void);
void put_byte(int c);
void put_work(int c);
void put_int(int c);
u_char *put_string(u_char *p);
u_char *put_ndata(u_char *p, int n);
void get_buf(void);
int get_byte(void);
int get_word(void);
#ifndef NO_GET_INT
int get_int(void);
#endif
int get_nstring(u_char *p, int n);
u_char *get_ndata(u_char *p, int n);

/* error.c */
int open_debug(void);
int open_error(void);
int open_log(void);
#if 0
void error_out(char *s, int p1, int p2, int p3, int p4, int p5);
void warning_out(char *s, int p1, int p2, int p3, int p4, int p5);
void logging_out(char *s, int p1, int p2, int p3, int p4, int p5);
void debug_out(int lvl, char *s, int p1, int p2, int p3, int p4, int p5);
#else /* correct solution is to make the functions take arbitrary parameters... */
void error_out ();
void warning_out ();
void logging_out ();
void debug_out ();
#endif

/* execute.c */
int make_full_path(char *path);
WorkArea *alloc_workarea(void);
void free_workarea(WorkArea *p);
void exec_connect(void);
void exec_disconnect(void);
void exec_opendict(void);
void exec_closedict(void);
void close_dictlist(DICTL *dictl);
void exec_openstdy(void);
void exec_closestdy(void);
void exec_stdysize(void);
void exec_ph2knj(int mb_flag);
void exec_cl2knj(int mb_flag);
void exec_nextcl(int mb_flag);
void exec_prevcl(int mb_flag);
void exec_cl2knj_cnt(int mb_flag);
void exec_cl2knj_all(int mb_flag);
void exec_study(void);
void exec_clstudy(int mb_flag);
void exec_adddict(int mb_flag);
void exec_deldict(int mb_flag);
void exec_getdict(int mb_flag);
void exec_nextdict(int mb_flag);
void exec_prevdict(int mb_flag);
void exec_makedict(void);
void exec_makestdy(void);
void exec_access(void);
void exec_makedir(void);
void exec_who(void);
void exec_kill(void);
void exec_quit(void);
void exec_version(void);
void exec_dictpass(void);
void exec_dictcmnt(void);
void exec_stdypass(void);
void exec_stdycmnt(void);
void exec_stdypara(void);
void execute_cmd(void);

/* setup.c */
void RcError(char *p);
void RcWarning(char *p);
int read_line(FILE *fp, char *buf, int len);
void read_runcmd(void);
void set_default(void);
void parse_arg(int argc, char **argv);
void preload_dict(void);
void preopen_dict(void);
int check_user(char *user, char *host);

#endif /* _SERVER_H_ */
