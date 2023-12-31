/*
 * Copyright (c) 1991-1994  Sony Corporation
 * Copyright (c) 1996 Hidekazu Kuroki <hidekazu at cs.titech.ac.jp>
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
 * $SonyRCSfile: setup.c,v $  
 * $SonyRevision: 1.3 $ 
 * $SonyDate: 1994/12/09 11:27:07 $
 *
 * $Id$
 */




#include "sj_sysvdef.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include "sj_typedef.h"
#include "sj_var.h"
#include "sj_dict.h"
#include "sj_struct.h"
#include "sj_rename.h"
#include "const.h"
#include "Struct.h"
#include "sj_global.h"
#include "kanakan.h"
#define NO_GET_INT
#include "server.h"

typedef	struct	strlist {
	unsigned char	*str1;
	unsigned char	*str2;
	struct	strlist	*link;
} StrList;

char	program_name	[PathNameLen];		
char	runcmd_file	[PathNameLen];		

char	*debug_file	= NULL;			
int	debug_level	= -1;			
int	fork_flag	= -1;			

int	max_client	= -1;			
char	*dict_dir	= NULL;			
StrList	*read_dict	= NULL;
StrList	*open_dict	= NULL;
char	*error_file	= NULL;			
char	*log_file	= NULL;
char	*port_name	= NULL;
#ifdef TLI
char	*port_number	= NULL;
#else
int	port_number	= -1;
#endif
char	*socket_name	= NULL;
#ifdef	LOCK_FILE
char	*lock_file	= NULL;
#endif
int	dir_mode	= -1;			
int	file_mode	= S_IRUSR|S_IWUSR;
StrList	*allow_user	= NULL;
#ifdef TLI
char    *proto_name     = NULL;
#endif

static	int	line_number;			


#ifdef	OLD
#else /* !OLD */
void RcError(char* p)
{
	fprintf(stderr, "%s: \"%s\", line %d: %s\r\n",
		program_name, runcmd_file, line_number, p);
	fflush(stderr);
	exit(1);
}

void RcWarning(char* p)
{
	fprintf(stderr, "%s: warning: \"%s\", line %d: %s\r\n",
		program_name, runcmd_file, line_number, p);
	fflush(stderr);
}
#endif /* !OLD */


static int cmpstr(unsigned char* src, unsigned char* dst)
{
	int	flg;
	int	c;

	flg = (isupper(*src)) ? -1 : 0;
	while (*src) {
		c = flg ? *dst : (isupper(*dst) ? tolower(*dst) : *dst);
		if (*src != c) return -1;
		src += 1;
		dst += 1;
	}
	if (*dst) return -1;
	return 0;
}



static	unsigned char	*get_str(unsigned char *p, void *pv_dst)
{
	char		**dst;

	dst = pv_dst;
	if (!*dst) {
		*dst = malloc(strlen((char *)p) + 1);
		if (!*dst) RcError("no more memory");
		strcpy(*dst, (char *)p);
	}
	while (*p++) ;
	return p;
}

static	unsigned char	*get_int(unsigned char *p, void *pv_dst)
{
	char	*fmt;
	int		*dst;

	dst = pv_dst;
	fmt = (*p == '0') ? "%o" : "%d";
	if (sscanf((char *)p, fmt, dst) != 1) RcError("bad integer");
	while (*p++) ;
	return p;
}

static	unsigned char	*get_flag(unsigned char *p, void *pv_dst)
{
	int		*dst;

	dst = pv_dst;
	if (sscanf((char *)p, "%d", dst) == 1)
		;
	else if (!cmpstr("on", p))
		*dst = -1;
	else if (!cmpstr("off", p))
		*dst = 0;
	else
		RcError("bad flag");
	while (*p++) ;
	return p;
}


static unsigned char* get_list(unsigned char* p, StrList** dst)
{
	StrList	*s1, *s2;

	if (*p) {
		s1 = (StrList *)malloc(sizeof(StrList));
		if (!s1) RcError("no more memory");
		s1 -> link = NULL;

		s1 -> str1 = (unsigned char *)malloc(strlen((char *)p) + 1);
		if (!(s1 -> str1)) RcError("no more memory");
		strcpy((char *)s1 -> str1, (char *)p);
		while (*p++) ;

		if (*p) {
			s1 -> str2 = (unsigned char *)malloc(strlen((char *)p) + 1);
			if (!(s1 -> str2)) RcError("no more memory");
			strcpy((char *)s1 -> str2, (char *)p);
			while (*p++) ;
		}
		else
			s1 -> str2 = NULL;

		if (*dst) {
			for (s2 = *dst ; s2 -> link ; s2 = s2 -> link)
				;
			s2 -> link = s1;
		}
		else
			*dst = s1;
	}

	return p;
}



struct	optlist {
	char	*optname;
	unsigned char	*(*optfunc)();
	void	*optarg;
} option[] = {
/*
 * Add option flag.
 * Because They are lacked in here. See document.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
	{"DebugOut",	get_str,	&debug_file},
	{"debugout",	get_str,	&debug_file},
	{"DebugLevel",	get_int,	&debug_level},
	{"debuglevel",	get_int,	&debug_level},
	{"ForkFlag",	get_flag,	&fork_flag},
	{"forkflag",	get_flag,	&fork_flag},

	{"PortName",	get_str,	&port_name},
	{"portname",	get_str,	&port_name},
#ifdef TLI
	{"PortNumber",	get_str,	&port_number},
	{"portnumber",	get_str,	&port_number},
	{"ProtoName",    get_str,        &proto_name},
	{"protoname",    get_str,        &proto_name},
#else
	{"PortNumber",	get_int,	&port_number},
	{"portnumber",	get_int,	&port_number},
#endif
	{"SocketName",	get_str,	&socket_name},
	{"socketname",	get_str,	&socket_name},
#ifdef	LOCK_FILE
	{"LockFile",	get_str,	&lock_file},
	{"lockfile",	get_str,	&lock_file},
#endif

	{"MaxClient",	get_int,	&max_client},
	{"maxclient",	get_int,	&max_client},
	{"DictDir",	get_str,	&dict_dir},
	{"dictdir",	get_str,	&dict_dir},
	{"ReadDict",	get_list,	&read_dict},
	{"readdict",	get_list,	&read_dict},
	{"OpenDict",	get_list,	&open_dict},
	{"opendict",	get_list,	&open_dict},
	{"ErrorOut",	get_str,	&error_file},
	{"errorout",	get_str,	&error_file},
	{"LogOut",	get_str,	&log_file},
	{"logout",	get_str,	&log_file},
	{"DirMode",	get_int,	&dir_mode},
	{"dirmode",	get_int,	&dir_mode},
	{"FileMode",	get_int,	&file_mode},
	{"filemode",	get_int,	&file_mode},
	{"AllowUser",	get_list,	&allow_user},
	{"allowuser",	get_list,	&allow_user},
	{0, 0, 0}
};



static	int	skip_blank(FILE *fp)
{
	int	c;

	c = getc(fp);
	while (c == ' ' || c == '\t') c = getc(fp);
	return c;
}

static	int	skip_line(FILE *fp)
{
	int	c;

	c = getc(fp);
	while (c != '\n' && c != EOF) c = getc(fp);
	return c;
}

static	int	readln(FILE *fp, char *p, int len)
{
	int	c;
	int	quote = EOF;
	int	flg = 0;

	c = skip_blank(fp);
	while (c != '\n' && c != EOF) {
		if (c == '\\') {
			if ((c = getc(fp)) == EOF) break;
			if (c == '\n') {
				line_number++; c = getc(fp); continue;
			}
			switch (c) {
			case 'n':	c = '\n'; break;
			case 't':	c = '\t'; break;
			case 'r':	c = '\r'; break;
			default:	break;
			}
		}
		else if (c == '^') {
			c = getc(fp);
			if (c < '@' || '_' < c) {
				ungetc(c, fp); c = '^';
			}
			else
				c -= '@';
		}
		else if (c == quote) {
			quote = EOF; c = getc(fp); continue;
		}
		else if (quote != EOF)
			;
		else if (c == '\'' || c == '"') {
			quote = c; c = getc(fp); continue;
		}
		else if (c == '#') {
			len -= 1; *p++ = 0; flg = 0;
			c = skip_line(fp); continue;
		}
		else if (c == ' ' || c == '\t') {
			len -= 1; *p++ = 0; flg = 0;
			c = skip_blank(fp); continue;
		}

		if (len > 2) { len -= 1; *p++ = c; flg = -1; }
		c = getc(fp);
	}
	line_number++;
	if (flg) *p++ = 0;
	*p++ = 0;

	return c;
}

int read_line(FILE* fp, char* buf, int len)
{
	int	flg;

	do {
		flg = readln(fp, buf, len);
		if (*buf) return 0;
	} while (flg != EOF);

	return EOF;
}



void read_runcmd(void)
{
	FILE	*fp;
	unsigned char	buf[BUFSIZ];
	struct	optlist *opt;
	unsigned char	*p;

	if (!(fp = fopen(runcmd_file, "r"))) {
		warning_out("Can't open run-command file \"%s\"", runcmd_file);
		return;
	}
	line_number = 0;
	while (read_line(fp, buf, sizeof(buf)) != EOF) {
		for (opt = option ; (p = (unsigned char *)opt -> optname) != NULL ; opt++)
			if (!cmpstr(p, buf)) break;
		if (p) {
			p = buf; while (*p++) ;
			p = (*opt->optfunc)(p, opt->optarg);
		}
		else
			RcError("undefined option");

		if (*p) RcError("too many argment");
	}

	fclose(fp);
}


static void set_defstr(char** d, char* s)
{
	if (*d == NULL && s) {
		*d = malloc(strlen(s) + 1);
		if (!*d) error_out("failed at memory allocation");
		strcpy(*d, s);
	}
}


static void set_defint(int* d, int s)
{
	if (*d < 0) *d = s;
}


void set_default(void)
{
	set_defstr(&debug_file,		DebugOutFile);
	set_defstr(&error_file,		ErrorOutFile);
	set_defstr(&dict_dir,		DictRootDir);
	set_defstr(&log_file,		LogOutFile);
	set_defstr(&port_name,		PortName);
	set_defstr(&socket_name,	SocketName);
#ifdef	LOCK_FILE
	set_defstr(&lock_file,		LockFile);
#endif
#ifdef TLI
        set_defstr(&proto_name,         ProtoName);
#endif

	set_defint(&debug_level,	DebugLevel);
	set_defint(&fork_flag,		ForkFlag);
	set_defint(&max_client,		MaxClientNum);
#ifdef TLI
	set_defstr(&port_number,	PortNumber);
#else
	set_defint(&port_number,	PortNumber);
#endif
	set_defint(&dir_mode,		DirectryMode);
	set_defint(&file_mode,		DictFileMode);
}


void parse_arg(int argc, char** argv)
{
	int	c;
	int	errflg = 0;
	char	*p;
	size_t	ret;

	extern	char	*optarg;
	extern	int	optind;

	p = (p = strrchr(argv[0], '/')) ? p + 1 : argv[0];
	strlcpy(program_name,	p, sizeof(program_name));
	strlcpy(runcmd_file,	RunCmdFile, sizeof(runcmd_file));

	while ((c = getopt(argc, argv, "f:")) != EOF) {
		switch (c) {
		case 'f':
			ret = strlcpy(runcmd_file, optarg, sizeof(runcmd_file));
			if (ret > sizeof(runcmd_file))
				errflg++;
			break;

		case '?':
		default:
			errflg++;
			break;
		}
	}

	if (errflg || optind < argc) {
		fprintf(stderr, "Usage: %s [-f RunCmdFile]\n", program_name);
		exit(1);
	}
}


void preload_dict(void)
{
	StrList	*p;
	char	filename[PathNameLen];

	work_base = (Global *)malloc(sizeof(Global));
	if (!work_base) {
		warning_out("can't load default dictionary(not enough memory)");
		return;
	}

	for (p = read_dict ; p ; p = p -> link) {
		strncpy(filename, (char *)p -> str1, sizeof(filename));
		filename[sizeof(filename) - 1] = '\0';
		if (make_full_path(filename))
			warning_out("too long filename \"%s\"", p -> str1);

		else if (opendict(filename, p -> str2))
			logging_out("load dictionary \"%s\"", filename);

		else
			warning_out("can't open dictionary \"%s\"", filename);
	}

	free(work_base);
}

void preopen_dict(void)
{
	StrList	*p;
	char	filename[PathNameLen];
	DICT	*dict;
	DICTL	*dictl;

	work_base = global_work_base = (Global *)malloc(sizeof(Global));
	if (!work_base) {
		warning_out("can't open default dictionary(not enough memory)");
		return;
	}
	memset(work_base, 0, sizeof(Global));

	for (p = open_dict ; p ; p = p -> link) {
		strncpy(filename, p -> str1, sizeof(filename));
		filename[sizeof(filename) - 1] = '\0';
		if (make_full_path(filename))
			warning_out("too long filename \"%s\"", p -> str1);

		else if ((dict = (DICT *)opendict(filename, p -> str2)) != NULL) {
			logging_out("open dictionary \"%s\"", filename);
		        if ((dictl = (DICTL *)malloc(sizeof *dictl)) == NULL) {
				closedict((DictFile *)dict);
				warning_out("can't open dictionary \"%s\"", filename);	
				continue;
			}
			dictl -> dict = dict;
			dictl -> next = dictlist;
			dictlist = dictl;

		} else
			warning_out("can't open dictionary \"%s\"", filename);
	}
}


static int str_match(char* s, char* d)
{
	while (*d) {
		if (*s == '*') {
			while (*s == '*') s++;
			if (!*s) return TRUE;
			while (*d) {
				if (str_match(s, d)) return TRUE;
				d++;
			}
			return FALSE;
		}
		if (*s != '?' && *s != *d) break;
		s++;
		d++;
	}
	return (!*s) ? TRUE : FALSE;
}


int check_user(char* user, char* host)
{
	StrList	*p;

	if (!allow_user) return TRUE;

	for (p = allow_user ; p ; p = p -> link) {
		if (!str_match(p -> str1, user)) continue;

		if (p -> str2)
			if (!str_match(p -> str2, host)) continue;

		return TRUE;
	}

	return FALSE;
}
