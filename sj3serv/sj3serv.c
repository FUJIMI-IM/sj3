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

/**
 * @file sj3serv.c
 * @brief かな漢字変換サーバーのメインプロセス。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pathnames.h"

const char	 default_conffile[] = _PATH_SJ3_CONF_FILE;
const char	*conffile = default_conffile;

__dead void
usage(void)
{
	extern char *__progname;

	fprintf(stderr, "usage: %s [-v] [-f file]\n", __progname);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	int verbose = 0;
	int c;

	while ((c = getopt(argc, argv, "f:v")) != -1) {
		switch (c) {
		case 'f':
			conffile = optarg;
			break;
		case 'v':
			verbose++;
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}

	return 0;
}
