/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */



#include "sj_sysvdef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <pwd.h>
#include <locale.h>
#include <unistd.h>
#include "sjtool.h"
#include "sj3lib.h"
#include "sj3dic.h"


static void _open_error(int err);
static void _close_error(int err);
static void init_env(void);
static void make_dicname(void);
static int  parsearg(int argc, char* argv[]);
static void usage(int ret);

#ifdef __sony_news
extern int _sys_code;
#else
int _sys_code = SYS_EUC;
#endif

static	struct	option	{
	char	*str;
	int	code;
} options[] = {
	{ "text",	EXEC_TEXT },
	{ "dict",	EXEC_DICT },
	{ "init",	EXEC_INIT },
	{ "verbose",	EXEC_VERBOSE },
	{ "host",	EXEC_SERVER },
	{ "server",	EXEC_SERVER },
	{ "force",	EXEC_FORCE },
	{ 0,		0 }
};

char	prog_name[LONGLENGTH];
char	home_dir[LONGLENGTH];
char	user_name[LONGLENGTH];
char	term_name[LONGLENGTH];
char	serv_name[LONGLENGTH];
char	file_name[LONGLENGTH];
char	dict_name[LONGLENGTH];
int	verbose_flag = 0;
int	init_flag = 0;
int	force_flag = 0;


int
main(int argc, char** argv)
{
	int	err;
	int	mode;

	if (!init_code()) {
		fprintf(stderr, "Can't excute on this locale\n");
		exit(1);
	}
	init_env();
	mode = parsearg(argc, argv);
	setsjserv(getenv("SJ3SERV"));
	getsjrc();
	make_dicname();

	/* XXX: is this progname intentional? or old variable left? */
	/* if ((err = sj3_open(serv_name, user_name, prog_name))) */
	if ((err = sj3_open(serv_name, user_name)))
		_open_error(err);

	switch (mode) {
	case EXEC_TEXT:
		dictdisp(file_name);
		break;

	case EXEC_DICT:
		dictmake(file_name);
		break;
	default:
		break;
	}

	if ((err = sj3_close())) _close_error(err);
}

static void
init_env(void)
{
	char	*un, *hp, *tn;
	struct	passwd	*pwd;

	prog_name[0] =
	home_dir[0]  =
	user_name[0] =
	term_name[0] =
	serv_name[0] =
	file_name[0] =
	dict_name[0] = '\0';

	un = getlogin();
	hp = getenv("HOME");

	setpwent();
	pwd = (un == NULL || *un == '\0') ? getpwuid(getuid()) : getpwnam(un);
	if (pwd != NULL) {
		strcpy(user_name, pwd -> pw_name);
		if (hp == NULL) hp = pwd -> pw_dir;
		endpwent();
	}
	if (hp != NULL) strcpy(home_dir, hp);

	if ((tn = getenv("TERM")) == NULL) {
		fprintf(stderr, "Can't getenv TERM\n\r");
		perror("getenv");
		exit(1);
	}
	strcpy(term_name, tn);
}

static void
usage(int ret)
{
	fprintf(stderr,
		"Usage: %s -{text|dict} [-H host_name] [file_name]\n",
		prog_name);
	exit(ret);
}

static int
parsearg(int argc, char* argv[])
{
	int	errflg = 0;
	int	i, j;
	char	*p, *q;
	struct	option	*opt;
	int	cmd;
	int	mode = 0;
	char	tmp[LONGLENGTH];
	extern char *strrchr();

	strcpy(prog_name, (p = strrchr(argv[0], '/')) ? p + 1 : argv[0]);

	for (i = 1 ; i < argc ; i++) {
		if (*argv[i] == '-') {
			for (p = argv[i]+1, q = tmp ; *p ; p++)
				*q++ = isupper(*p) ? tolower(*p) : *p;
			*q = 0;
			j = strlen(tmp);
			cmd = 0;
			for (opt = options ; opt -> code ; opt++) {
				if (strncmp(opt -> str, tmp, j))
					continue;
				else if (cmd) {
					errflg++; break;
				}
				else
					cmd = opt -> code;
			}
			if (errflg) break;
			if (cmd == 0) { errflg++; break; }
			switch (cmd) {
			case EXEC_TEXT:
			case EXEC_DICT:
				if (mode)
					errflg++;
				else
					mode = cmd;
				break;

			case EXEC_INIT:
				init_flag++; break;

			case EXEC_VERBOSE:
				verbose_flag++; break;

			case EXEC_SERVER:
				if (++i >= argc) { errflg++; break; }
				setsjserv(argv[i]);
				break;

			case EXEC_FORCE:
				force_flag++; break;

			default:
				errflg++; break;
			}
		}
		else if (file_name[0]) {
			errflg++; break;
		}
		else
			strcpy(file_name, argv[i]);
	}

	if (errflg || mode == 0) usage(1);

	return mode;
}

static void
make_dicname(void)
{
	if (dict_name[0] != '\0') return;

	strcpy(dict_name, home_dir);
	strcat(dict_name, "/");
	strcat(dict_name, "sj2usr.dic");
}

void
setdicname(char* dictname)
{
	if (dict_name[0] != '\0') return;
	if (dictname == NULL) return;
	strcpy(dict_name, dictname);
}

void
setsjserv(char* hostname)
{
	if (serv_name[0] != '\0') return;
	if (hostname == NULL) return;
	strcpy(serv_name, hostname);
}

struct	errlist	{
	int	code;
	char	*msg;
	int	flg;
};

static void
_error_and(int err, struct errlist* list)
{
	int	flag = 0;

	while (list -> code) {
		if (err & list -> code) {
			error_out(list -> msg);
			err &= ~(list -> code);
			if (list -> flg) flag++;
		}
		list++;
	}
	if (flag) exit(1);
}

static void
_open_error(int err)
{
	static	struct	errlist	err_msg[] = {
	{ SJ3_SERVER_DEAD,	"\245\265\241\274\245\320\244\254\273\340\244\363\244\307\244\244\244\336\244\271",			1 },
	{ SJ3_CONNECT_ERROR,	"\245\265\241\274\245\320\244\310\300\334\302\263\244\307\244\255\244\336\244\273\244\363\244\307\244\267\244\277",		1 },
	{ SJ3_ALREADY_CONNECTED,	"\245\265\241\274\245\320\244\310\300\334\302\263\272\321\244\307\244\271",			1 },
	{ SJ3_CANNOT_OPEN_MDICT,	"\245\341\245\244\245\363\274\255\275\361\244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363",	0 },
	{ SJ3_CANNOT_OPEN_UDICT,	"\245\346\241\274\245\266\274\255\275\361\244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363",	1 },
	{ SJ3_CANNOT_OPEN_STUDY,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\254\245\252\241\274\245\327\245\363\244\307\244\255\244\336\244\273\244\363",	0 },
	{ SJ3_CANNOT_MAKE_UDIR,	"\245\346\241\274\245\266\241\274\245\307\245\243\245\354\245\257\245\310\245\352\244\254\272\356\300\256\244\307\244\255\244\336\244\273\244\363",	1 },
	{ SJ3_CANNOT_MAKE_UDICT,	"\245\346\241\274\245\266\274\255\275\361\244\254\272\356\300\256\244\307\244\255\244\336\244\273\244\363",		1 },
	{ SJ3_CANNOT_MAKE_STUDY,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\254\272\356\300\256\244\307\244\255\244\336\244\273\244\363",		0 },
	{ -1,			"\245\250\245\351\241\274",				1 },
	{ 0, 0, 0 }
	};

	_error_and(err, err_msg);
}

static void
_close_error(int err)
{
	static	struct	errlist	err_msg[] = {
	{ SJ3_SERVER_DEAD,	"\245\265\241\274\245\320\241\274\244\254\273\340\244\363\244\307\244\244\244\336\244\271",		1 },
	{ SJ3_DISCONNECT_ERROR,	"\300\332\244\352\312\374\244\267\244\307\245\250\245\351\241\274\244\254\244\242\244\352\244\336\244\267\244\277",		1 },
	{ SJ3_NOT_CONNECTED,	"\245\265\241\274\245\320\244\310\300\334\302\263\244\265\244\354\244\306\244\244\244\336\244\273\244\363",		1 },
	{ SJ3_NOT_OPENED_MDICT,	"\245\341\245\244\245\363\274\255\275\361\244\317\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\273\244\363",	0 },
	{ SJ3_NOT_OPENED_UDICT,	"\245\346\241\274\245\266\274\255\275\361\244\317\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\273\244\363",	1 },
	{ SJ3_NOT_OPENED_STUDY,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\317\245\252\241\274\245\327\245\363\244\265\244\354\244\306\244\244\244\336\244\273\244\363",	0 },
	{ SJ3_CLOSE_MDICT_ERROR,	"\245\341\245\244\245\363\274\255\275\361\244\254\245\257\245\355\241\274\245\272\244\307\244\255\244\336\244\273\244\363",	0 },
	{ SJ3_CLOSE_UDICT_ERROR,	"\245\346\241\274\245\266\274\255\275\361\244\254\245\257\245\355\241\274\245\272\244\307\244\255\244\336\244\273\244\363",	1 },
	{ SJ3_CLOSE_STUDY_ERROR,	"\263\330\275\254\245\325\245\241\245\244\245\353\244\254\245\257\245\355\241\274\245\272\244\307\244\255\244\336\244\273\244\363",	0 },
	{ -1,			"\245\250\245\351\241\274",				1 },
	{ 0, 0, 0 }
	};

	_error_and(err, err_msg);
}
