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

/*
 * $SonyRCSfile: ttytest.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:31 $
 */


#include <signal.h>
#include <termio.h>
#include <stdio.h>
#include <fcntl.h>
#include <stropts.h>
#include <sys/types.h>

#define PTY_MASTER "/dev/ptmx"
#define BUFFSIZE 512
#define err_sys(x) fprintf(stderr, (x))
#define err_quit(x) { \
	             fprintf(stderr, (x)); \
	             exit(0); \
                    }


static struct termio tty_termio;
static struct termio tty_mode;
static int sigcaught;

exec_shell(fd, argv, envp)
int fd;
char **argv;
char **envp;
{
	char *shell;
	char *getenv(), *rindex();
	struct termio b;

	close(0); close(1); close(2);
	if (dup(fd) != 0 || dup(fd) != 1 || dup(fd) != 2)
	  err_sys("dup error");
	close(fd);

	if ((shell = getenv("SHELL")) == NULL)
	  shell = "/bin/sh";
	if ((argv[0] = rindex(shell, '/')) != NULL)
	  argv[0]++;
	else
	  argv[0] = shell;

	if (ioctl(0, TCGETA, (char *) &b) < 0)
	  return(-1);
fprintf(stderr, "erase=%x\n", b.c_cc[VERASE]);
	execve(shell, argv, envp);
	err_sys("execve error");
}

pass_all(fd, childpid)
int fd;
int childpid;
{
	int newpid, nread;
	int sig_term();
	char buff[BUFFSIZE];

	if ((newpid = fork()) < 0) {
		err_sys("parent1: can't fork");
	} else if (newpid == 0) {
		for ( ; ; ) {
			nread = read(0, buff, BUFFSIZE);
			if (nread < 0)
			  err_sys("read error fram stdin");
			else if (nread == 0)
			  break;

			if (write(fd, buff, nread) != nread)
			  err_sys("writen error to stream pipe");
		}
		kill(getppid(), SIGTERM);
		exit(0);
	}


	sigcaught = 0;


	for ( ; ; ) {
		if ((nread = read(fd, buff, BUFFSIZE)) <= 0)
		  break;

		if (write(1, buff, nread) != nread)
		  err_sys("write error to stdout");
	}

	if (sigcaught == 0)
	  kill(newpid, SIGTERM);

	return;
}

int
pty_master()
{
	int master_fd;
	
	if ((master_fd = open(PTY_MASTER, O_RDWR)) < 0)
	  return(-1);

	return(master_fd);
}

int 
pty_slave(master_fd)
int master_fd;
{
	int slave_fd;
	char *slavename;
	int grantpt();
	int unlockpt();
	char *ptsname();

	if (grantpt(master_fd) < 0) {
		close(master_fd);
		return(-1);
	}

	if (unlockpt(master_fd) < 0) {
		close(master_fd);
		return(-1);
	}
	
	slavename = ptsname(master_fd);
	if (slavename == NULL) {
		close(master_fd);
		return(-1);
	}
	
	slave_fd = open(slavename, O_RDWR);
	if (slave_fd < 0) {
		close(master_fd);
		return(-1);
	}
	
	if (ioctl(slave_fd, I_PUSH, "ptem") < 0 ) {
		close(master_fd);
		return(-1);
	}

	if (ioctl(slave_fd, I_PUSH, "ldterm") < 0) {
		close(master_fd);
		return(-1);
	}
	return(slave_fd);
}

int 
tty_raw(fd)
int fd;
{
	struct termio temp_mode;

	if (ioctl(fd, TCGETA, (char *) &temp_mode) < 0)
	  return(-1);

	tty_mode = temp_mode;
	
	temp_mode.c_iflag = 0;
	temp_mode.c_oflag &= ~OPOST;
	temp_mode.c_lflag &= ~(ISIG | ICANON | ECHO | XCASE);
	temp_mode.c_cflag &= ~(CSIZE | PARENB);
	temp_mode.c_cflag |= CS8;
	temp_mode.c_cc[VMIN] = 1;
	temp_mode.c_cc[VTIME] = 1;

	if (ioctl(fd, TCSETA, (char *) &temp_mode) <0)
	  return(-1);

	return(0);
}

int 
tty_reset(fd)
int fd;
{
	if (ioctl(fd, TCSETA, (char *) &tty_mode) < 0)
	  return(-1);

	return(0);
}

int tty_getmode(oldfd)
int oldfd;
{
	if (ioctl(oldfd, TCGETA, (char *) &tty_termio) < 0)
	  return(-1);
fprintf(stderr, "tty_getmode erase=%x\n", tty_termio.c_cc[VERASE]);
	return(0);
}

int 
tty_setmode(newfd)
int newfd;
{
	if (ioctl(newfd, TCSETA, (char *) &tty_termio) < 0)
	  return(-1);

fprintf(stderr, "tty_setmode erase=%x\n", tty_termio.c_cc[VERASE]);
	return(0);
}

main(argc, argv, envp)
int argc;
char **argv;
char **envp;
{
	int master_fd, slave_fd, childpid;

	if (!isatty(0) || !isatty(1))
	  err_quit("stdin and stdout must be a terminal");

	if ((master_fd = pty_master()) < 0)
	  err_sys("can't open master pty");
	if(tty_getmode(0) < 0)
	  err_sys("can't get tty mode of standard input");

	if ((childpid = fork()) < 0)
	  err_sys("can't fork");
	else if (childpid == 0) {
		setpgrp();
		if ((slave_fd = pty_slave(master_fd)) < 0)
		  err_sys("can't open pty slave");
		close(master_fd);
		if (tty_setmode(slave_fd) < 0)
		  err_sys("can't set tty mode of pty slave");

		exec_shell(slave_fd, argv, envp);
	}


	if (tty_raw(0) < 0)
	  err_sys("tty_raw error");
	pass_all(master_fd, childpid);

	if (tty_reset(0) < 0)
	  err_sys("tty_reset error");

	exit(0);
