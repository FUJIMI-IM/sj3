/*
 * Copyright (C) 1995 by Software Research Associates, Inc.
 *      Author: Y. Kawabe <kawabe@sra.co.jp>
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
 * IN NO EVENT SHALL SOFTWARE RESEARCH ASSOCIATES BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Software Research
 * Associates shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without prior
 * written authorization from Software Research Associates.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Const.h"
#include "sj3err.h"
#include "sj3lib.h"

int
main(int argc, char **argv)
{
	SJ3_CLIENT_ENV	 clnt;
	SJ3_WHO_STRUCT	 who[64];
	char		 ver[256];
	char		 pname[256];
	char		*hname;
	char		*uname;

	int	 i, n;
	char	*p;

	if ((argc > 1 && argv[1][0] == '-') || argc > 2) {
		fprintf(stderr, "usage: %s [server]\n", argv[0]);
		exit(1);
	}

	hname = getenv("SJ3SERV");
	uname = getenv("USER");

	if (argc > 1)
		hname = argv[1];
	if (uname == NULL)
		uname = getlogin();
	if (uname == NULL)
		uname = "unknown";
	sprintf(pname, "%d.sj3stat", getpid());

	if (sj3_make_connection(&clnt, hname, uname, pname) == ERROR) {
		char		*server, *status;
		extern int	 sj3_error_number;

		server = hname ? hname : "localhost";
		switch (sj3_error_number) {
		case SJ3_ServerDown: status = "down"; break;
		case SJ3_GetHostByName: status = "unknown"; break;
		default: status = "dead"; break;
		}
		fprintf(stderr, "Server '%s' is %s.\n", server, status);
		return 1;
	}

	sj3_version(&clnt, ver, sizeof(ver));
	n = sj3_who(&clnt, who, sizeof(who)/sizeof(who[0]));

	for (p = ver; *p; p++) {
		while (*p)
			putc(*p++, stdout);
		fprintf(stdout, "%%, ");
	}
	fprintf(stdout, "%d users.\n", n);

/*
 * Change hostname entry's length.
 * Because it's so short.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
	fprintf(stdout, "%-16s %-32s %-8s %s\n",
	    "(USER)", "(HOST)", "(FD)", "(CLIENT)");
	for (i = 0; i < n; i++) {
		fprintf(stdout, "%-16s %-32s %-8d %s\n", who[i].username,
		    who[i].hostname, who[i].fd, who[i].progname);
	}
	(void)sj3_erase_connection(&clnt);
	return 0;
}
