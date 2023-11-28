/*
 * Copyright (c) 1994  Sony Corporation
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

#include <locale.h>
#include <pwd.h>
#include <stdio.h>

#include "sj3lib.h"
#include "wchar16.h"

#define	RETURN(c)	((c == '\n') || (c == '\r'))
#define	RKFILE		"/usr/local/lib/sj3/sjrk"
#define	LOCALHOST	"localhost"
#define	HBUFSIZ		512
#define	QBUFSIZ		256

char	*pname;
char	*hname;
int	 current_locale = LC_CTYPE_EUC;

void
usage()
{
	fprintf(stderr, "Usage: %s [-v] [-H hostname]\n", pname);
	exit(1);
}

int
main(int argc, char **argv)
{
	struct bunsetu	 bun[HBUFSIZ];
	unsigned char	 alphabet[QBUFSIZ];
	unsigned char	 yomi[HBUFSIZ];
	unsigned char	 kanji[BUFSIZ];
	extern char	*optarg;
	char		*rindex(), *getlogin();
	char		*loc, *uname;
	struct passwd	*getpwuid();
	struct passwd	*pwd;
	int		 count = 1;
	int		 c, ret, vflag;

	loc = setlocale(LC_CTYPE, "");

	if (strcmp(loc, "ja_JP.SJIS") == 0) {
		current_locale = LC_CTYPE_SHIFTJIS;
	} else if (strncmp(loc, "ja", 2) != 0 && strncmp(loc, "Ja", 2) != 0) {
		fprintf(stderr, "Error: %s is not japanese locale\n", loc);
		exit(1);
	}
	vflag = 0;
	hname = NULL;

	print_head();

	pname = argv[0];

	while ((c = getopt(argc, argv, "vk:H:")) != EOF) {
		switch (c) {
		case 'v':
			vflag++;
			break;
		case 'H':
			hname = optarg;
			break;
		case 'k':
			fprintf(stderr, "optarg = %s\n", optarg);
			count = atoi(optarg);
			break;
		case '?':
		default:
			usage();
			break;
		}
	}

	uname = getlogin();
	if (uname == NULL || *uname == '\0') {
		setpwent();
		if ((pwd = getpwuid(getuid())) != NULL) {
			uname = pwd->pw_name;
			endpwent();
		} else {
			fprintf(stderr, "Can't get user name\n");
			exit(1);
		}
	}

	if (ret = sj3_rkinit(RKFILE)) {
		if (ret == 1) {
			fprintf(stderr,
			    "Can't open Romaji Kana Rule file %s\n",
			    RKFILE);
		} else {
			fprintf(stderr, "Error sj3_rkinit\n");
		}
		exit(1);
	}
	ret = sj3_open(hname, uname);
	if (hname == NULL)
		hname = LOCALHOST;
	if (ret != 0) {
		if (ret & SJ3_SERVER_DEAD) {
			fprintf(stderr, "%s:sj3serv dead\n", hname);
			exit(1);
		} else if (ret & SJ3_ALREADY_CONNECTED) {
			fprintf(stderr,
			    "Already connected %s:sj3serv\n", hname);
			exit(1);
		} else if (ret & SJ3_CONNECT_ERROR) {
			fprintf(stderr, "Can't connect %s:sj3serv\n",
			    hname);
			exit(1);
		} else if (ret & SJ3_CANNOT_OPEN_MDICT) {
			fprintf(stderr,
			    "Can't open sj3 main dictionary\n");
			(void)sj3_close();
			exit(1);
		}
		fprintf(stderr,
		    "Warning sj3_open error (return 0x%x)\n", ret);
	}

	while (1) {
		fprintf(stdout, "[Input Alphabet] ");
		if (fgets((char *)alphabet, QBUFSIZ, stdin) == NULL)
			break;
		if (RETURN(alphabet[0]))
			break;

		rkconvs(yomi, alphabet);

		ret = sj3_getkan_mb(yomi, bun, kanji, BUFSIZ);
		if (ret == -1) {
			fprintf(stderr, "sj3_getkan: disconnected %s:sj3serv\n",
			    hname);
			exit(1);
		}

		if (vflag)
			pr_kanji(bun, ret);

		if (bun[ret - 1].destlen == 0)
			strcat(kanji, bun[ret - 1].srcstr);
		fprintf(stdout, "[Output Yomi ]:[%s]\n", yomi);
		fprintf(stdout, "[Output Kanji]:[%s]\n", kanji);
	}

	(void)sj3_close();
}

void
rkconvs(unsigned char *yomi, unsigned char *roma)
{
	int		 i, len, rlen;
	unsigned char	 rbuf[16];
	unsigned char	*p, *q, *yp;
	unsigned char	 ybuf[32];
	unsigned char	 tmp[HBUFSIZ];

	yp = tmp;
	p = rbuf;
	rlen = 0;
	while (*roma != '\0' && !RETURN(*roma)) {
		*p++ = *roma++;
		*p = '\0';
		rlen++;
		while (1) {
			if ((len = sj3_rkconv_mb(rbuf, ybuf)) > 0) {
				q = ybuf;
				while (*q != '\0')
					*yp++ = *q++;
				i = rlen;
				rlen = strlen(rbuf);
				p = rbuf;
				p += rlen;
				if (len >= i)
					break;
			} else if (len < 0) {
				*yp++ = rbuf[0];
				p = rbuf;
				q = (p + 1);
				while (*q != '\0')
					*p++ = *q++;
				*p = '\0';
				rlen--;
			} else {
				break;
			}
		}
	}

	if (rlen > 0) {
		*p++ = '\n';
		*p = '\0';
		rlen++;
		while (rbuf[0] != '\0') {
			if ((len = sj3_rkconv_mb(rbuf, ybuf)) > 0) {
				q = ybuf;
				while (*q != '\0')
					*yp++ = *q++;

				rlen = strlen(rbuf);
				p = rbuf;
				p += rlen;
			} else {
				*yp++ = rbuf[0];
				p = rbuf;
				q = (p + 1);
				while (*q != '\0')
					*p++ = *q++;
				*p = '\0';
			}
		}
		yp--;
	}
	*yp = '\0';

	sj3_hantozen_mb(yomi, tmp);
}

void
print_head()
{
	fprintf(stdout, "\t*--------------------------------------------*\n");
	fprintf(stdout, "\t* Sample Program for                         *\n");
	fprintf(stdout, "\t* New Japanese Convert Library               *\n");
	fprintf(stdout, "\t* SJ3LIB                                     *\n");
	fprintf(stdout, "\t*                                            *\n");
	fprintf(stdout, "\t*    Terminator of Input : Newline-code      *\n");
	fprintf(stdout, "\t*                                            *\n");
	fprintf(stdout, "\t*    If you want to end this process,        *\n");
	fprintf(stdout, "\t*    input Newline-code or Control-D.        *\n");
	fprintf(stdout, "\t*--------------------------------------------*\n");
}

void
str_make(unsiged char *out_str, unsigned char *in_str, int len)
{
	strncpy(out_str, in_str, len);
	out_str += len;
	*out_str = '\0';
}

void
pr_douon(unsigned char *yomi)
{
	struct douon	douon[BUFSIZ];
	int		douon_cnt, i;

	douon_cnt = sj3_getdouon_mb(yomi, douon);
	if (douon_cnt < 0) {
		fprintf(stderr, "sj3_getdouon: disconnected %s:sj3serv\n",
		    hname);
		exit(1);
	}
	fprintf(stdout, "\t\tsj3_getdouon(yomi, douon)");
	fprintf(stdout, "...Douon count : %d\n", douon_cnt);
	for (i = 0; i < douon_cnt; i++) {
		fprintf(stdout, "\t\t--- * Douon : %d * ---\n", i);
		fprintf(stdout, "\t\t\tdlen : %d\n", douon[i].dlen);
		fprintf(stdout, "\t\t\tddata:[%s]\n", douon[i].ddata);
	}
}

void
pr_bunsetu(struct bunsetu *now_bun, int bunsetu_cnt)
{
	unsigned char	 tmp_str[BUFSIZ];
	unsigned char	*yomi;

	fprintf(stdout, "\t--- < Bunsetu : %d > ---\n", bunsetu_cnt);
	fprintf(stdout, "\t\tsrclen : %d\n", now_bun->srclen);
	str_make(tmp_str, now_bun->srcstr, now_bun->srclen);
	fprintf(stdout, "\t\tsrcstr:[%s]\n", tmp_str);

	yomi = tmp_str;
	if (now_bun->destlen > 0)
		pr_douon(yomi);
	else
		fprintf(stderr, "\t\tNot Conversion Bunsetsu\n");

	fprintf(stdout, "\t\tdestlen : %d\n", now_bun->destlen);
	str_make(tmp_str, now_bun->deststr, now_bun->destlen);
	fprintf(stdout, "\t\tdeststr:[%s]\n", tmp_str);
}

void
pr_kanji(struct bunsetu *bun, int bnum)
{
	int i;

	fprintf(stdout, "\tsj3_getkan(yomi, kanji)");
	fprintf(stdout, "...Bunsetu count : %d\n", bnum);
	for (i = 0; i < bnum; i++)
		pr_bunsetu(&(bun[i]), i);
}
