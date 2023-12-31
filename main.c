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

#include "config.h"

#include "sj_sysvdef.h"

#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "const.h"
#include "server.h"
#include "kanakan.h"

extern int	 fork_flag;
extern char	*lock_file;

char	*version_number = "2.08C";
char	*time_stamp = "Mon Mar 23 16:42:59 JST 1998";

static void opening(void)
{
	printf("Kana-Kanji Conversion Server Version %s\r\n", version_number);
	printf("Copyright (c) 1990-1995 Sony Corporation\r\n");
	printf("Created at %s\r\n", time_stamp);
}

#ifdef	LOCK_FILE

int make_lockfile(void)
{
	int	fd;

	fd = open(lock_file, O_CREAT | O_EXCL, 0600);
	if (fd < 0)
		return ERROR;
	close(fd);
	return 0;
}

int erase_lockfile(void)
{
	return unlink(lock_file);
}
#endif

static void signal_handler(int sig)
{
	warning_out("signal %d catched.", sig);
}

static void terminate_handler(int sig)
{
	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}

void server_terminate(void)
{
	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}

static void exec_fork(void)
{
	int	tmp;

	if (fork_flag && (tmp = fork())) {
		if (tmp < 0) {
			fprintf(stderr, "Can't fork child process\r\n");
			fflush(stderr);
		} else {
			signal(SIGCHLD, _exit);
			signal(SIGHUP, SIG_IGN);
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGTSTP, SIG_IGN);
			signal(SIGTERM, _exit);
			wait(0);
			_exit(0);
		}
	}

	signal(SIGHUP, (void (*)())signal_handler);

	signal(SIGINT, (void (*)())terminate_handler);
	signal(SIGQUIT, (void (*)())signal_handler);
	signal(SIGTERM, (void (*)())terminate_handler);
	if (fork_flag)
		signal(SIGTSTP, SIG_IGN);
}

static void leave_tty(void)
{
	int	tmp;

	tmp = open_error();
	tmp |= open_log();
	tmp |= open_debug();
	kill(getppid(), SIGTERM);
	fclose(stdin);
	fclose(stdout);
	if (!tmp)
		fclose(stderr);

	if (fork_flag) {
		if ((tmp = open("/dev/tty", O_RDWR)) >= 0) {
			ioctl(tmp, TIOCNOTTY, 0);
			close(tmp);
		}
	}
}

int main(int argc, char **argv)
{
	opening();
#ifndef __NetBSD__
	if (setuid(geteuid())) {
		fprintf(stderr, "error at setuid.\r\n");
		exit(1);
	}
#endif

	parse_arg(argc, argv);
	read_runcmd();
	set_default();

#ifdef	LOCK_FILE
	if (make_lockfile() == ERROR) {
		fprintf(stderr, "Already running...\r\n");
		exit(1);
	}
#endif

	exec_fork();

	socket_init();
	open_socket();

	leave_tty();

	preload_dict();
	preopen_dict();
	communicate();

	close_socket();
	sj_closeall();
#ifdef	LOCK_FILE
	erase_lockfile();
#endif
	exit(0);
}
