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

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include <netinet/in.h>

#include <errno.h>
#include <netdb.h>
#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sj_kcnv.h"

#ifdef SVR4
#define signal sigset
#endif

#if defined(__FD_SET) && !defined(FD_SET)
# define FD_SET __FD_SET
# define FD_ZERO __FD_ZERO
# define FD_CLR __FD_CLR
# define FD_ISSET __FD_ISSET
#endif

#ifndef SOMAXCONN
# define SOMAXCONN 5
#endif

int	 client_num = 0;
Client	*client = NULL;
Client	*cur_client;

int	maxfds = 0;
#ifdef SVR4
struct pollfd poll_fds[_NFILE];
#else
fd_set	fds_org = { 0 };
#endif
jmp_buf	client_dead;
int	client_fd;

extern char	*socket_name;
extern char	*port_name;
extern int	port_number;
extern int	max_client;

static int	fd_inet = -1;
static int	fd_unix = -1;

static char	putbuf[BUFSIZ];
static char	getbuf[BUFSIZ];
static int	putpos = 0;
static int	buflen = 0;
static int	getpos = 0;

void
socket_init(void)
{
#ifdef SVR4
	int i;

	for (i = 0; i < _NFILE; i++) {
		poll_fds[i].fd = -1;
		poll_fds[i].events = 0;
		poll_fds[i].revents = 0;
	}
#else
	FD_ZERO(&fds_org);
#endif
	client_num = 0;
	maxfds = 0;
}

static void
set_fd(int fd)
{
#ifdef SVR4
	poll_fds[maxfds].fd = fd;
	poll_fds[maxfds].events = POLLIN;
	maxfds++;
#else
	FD_SET(fd, &fds_org);
	if (fd >= maxfds)
		maxfds = fd + 1;
#endif
}

static void
clr_fd(int fd)
{
#ifdef SVR4
	int i, j;

	for (i = 0; i < maxfds; i++) {
		if (poll_fds[i].fd == fd) {
			maxfds--;
			for (j = i; j < maxfds; j++) {
				poll_fds[j].fd = poll_fds[j + 1].fd;
				poll_fds[j].events = poll_fds[j + 1].events;
				poll_fds[j].revents = poll_fds[j + 1].revents;
			}
			poll_fds[j].fd = -1;
			poll_fds[j].events = 0;
			poll_fds[j].revents = 0;
			break;
		}
	}
#else
	FD_CLR(fd, &fds_org);
	if (maxfds == fd + 1) {
		while (--fd >= 0) {
			if (FD_ISSET(fd, &fds_org))
				break;
		}
		maxfds = fd + 1;
	}
#endif
}


static void
open_af_unix(void)
{
	struct sockaddr_un	sunix;

	unlink(socket_name);

	memset((char *)&sunix, '\0', sizeof(sunix));
	sunix.sun_family = AF_UNIX;
	strcpy(sunix.sun_path, socket_name);

	if ((fd_unix = socket(AF_UNIX, SOCK_STREAM, 0)) == ERROR) {
		unlink(socket_name);
		fprintf(stderr, "Can't create AF_UNIX socket\n");
		exit(1);
	}

	if (bind(fd_unix, (struct sockaddr *)&sunix,
	    strlen(sunix.sun_path) + 2) == ERROR) {
		if (errno == EADDRINUSE) {
			fprintf(stderr, "Port '%s' is already in use\n",
			    socket_name);
		} else {
			fprintf(stderr, "Can't bind AF_UNIX socket\n");
		}
		shutdown(fd_unix, 2);
		close(fd_unix);
		unlink(socket_name);
		exit(1);
	}

	if (listen(fd_unix, SOMAXCONN) == ERROR) {
		shutdown(fd_unix, 2);
		unlink(socket_name);
		close(fd_unix);
		fprintf(stderr, "Can't listen AF_UNIX socket\n");
		exit(1);
	}
	set_fd(fd_unix);
}


static void
open_af_inet(void)
{
	struct sockaddr_in	 sin;
	struct servent		*sp;
	u_short			 port;
	int	true = 1;

	if ((sp = getservbyname(port_name, "tcp")) != NULL)
		port = ntohs(sp->s_port);
	else
		port = port_number;

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((fd_inet = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		fprintf(stderr, "Can't create AF_INET socket\n");
		exit(1);
	}

	setsockopt(fd_inet, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true));

	if (bind(fd_inet, (struct sockaddr *)&sin, sizeof(sin)) == ERROR) {
		if (errno == EADDRINUSE) {
			if (sp) {
				fprintf(stderr,
				    "Port '%s(%d)' is already in use\n",
				    port_name, port);
			} else {
				fprintf(stderr,
				    "Port %d is already in use\n",
				    port);
			}
		} else {
			fprintf(stderr, "Can't bind AF_INET socket\n");
		}
		shutdown(fd_inet, 2);
		close(fd_inet);
		exit(1);
	}

	if (listen(fd_inet, SOMAXCONN) == ERROR) {
		shutdown(fd_inet, 2);
		close(fd_inet);
		fprintf(stderr, "Can't listen AF_INET socket\n");
		exit(1);
	}
	set_fd(fd_inet);
}

void
open_socket(void)
{
	open_af_inet();
	open_af_unix();
}

static int
connect_af_unix(void)
{
	struct sockaddr_un	sunix;
	int			i = sizeof(sunix);
	int			fd;

	if ((fd = accept(fd_unix, (struct sockaddr *)&sunix, &i)) == ERROR)
		warning_out("Can't accept AF_UNIX socket");
	return fd;
}

static int
connect_af_inet(void)
{
	struct sockaddr_in	sin;
	int			i = sizeof(sin);
	int	fd;

	if ((fd = accept(fd_inet, (struct sockaddr *)&sin, &i)) == ERROR)
		warning_out("Can't accept AF_INET socket");
	return fd;
}

static void
close_af_unix(void)
{
	struct sockaddr_un	sunix;
	int			true = 1;
	int			i;

	ioctl(fd_unix, FIONBIO, &true);
	for (; ;) {
		i = sizeof(sunix);
		if (accept(fd_unix, (struct sockaddr *)&sunix, &i) < 0)
			break;
	}
	shutdown(fd_unix, 2);
	close(fd_unix);
	unlink(socket_name);
	clr_fd(fd_unix);
}

static void
close_af_inet(void)
{
	struct sockaddr_in	sin;
	int	true = 1;
	int	i;

	ioctl(fd_inet, FIONBIO, &true);
	for (; ;) {
		i = sizeof(sin);
		if (accept(fd_inet, (struct sockaddr *)&sin, &i) < 0)
			break;
	}
	shutdown(fd_inet, 2);
	close(fd_inet);
	clr_fd(fd_inet);
}

void
close_socket(void)
{
	close_af_unix();
	close_af_inet();
}

static void
#ifdef SVR4
connect_client(struct pollfd *readfds)
#else
connect_client(fd_set *readfds)
#endif
{
	int	fd;
	Client   *client_tmp;

#ifdef SVR4
	int     i;

	for (i = 0; i < maxfds; i++) {
		if ((readfds[i].fd == fd_inet) &&
		    (readfds[i].revents == POLLIN)) {
			fd = connect_af_inet();
			readfds[i].revents = 0;
			break;
		}
		if ((readfds[i].fd == fd_unix) &&
		    (readfds[i].revents == POLLIN)) {
			fd = connect_af_unix();
			readfds[i].revents = 0;
			break;
		}
	}
	if (i == maxfds)
		return;
#else /* SVR4 */
	if (FD_ISSET(fd_inet, readfds))
		fd = connect_af_inet();
	else if (FD_ISSET(fd_unix, readfds))
		fd = connect_af_unix();
	else
		return;
#endif /* SVR4 */
	if (fd == ERROR)
		return;

	if (client_num >= MaxClientNum || client_num >= max_client) {
		warning_out("No more client");
	} else {
		debug_out(1, "client create(%d)\r\n", fd);
		logging_out("connect to client on %d", fd);

		if ((client_tmp = (Client *)malloc(sizeof(Client))) != NULL) {
			memset(client_tmp, 0, sizeof(Client));
			client_tmp->fd = fd;
			client_tmp->next = client;
			client = client_tmp;
			client_num++;

			set_fd(fd);

			return;
		}
		warning_out("No more client(Can't alloc)");
	}
}

static Client *
disconnect_client(Client *cp)
{
	WorkArea	*wp;
	StdyFile	*sp;
	int		 fd;
	Client		*cl_before, *cl_tmp;

	if ((sp = cp->stdy) != NULL)
		closestdy(sp);
	if ((wp = cp->work) != NULL)
		free_workarea(wp);

	debug_out(1, "client dead(%d)\r\n", cp->fd);
	logging_out("disconnect from client on %d", cp->fd);
	if (cp == client) {
		client = cp->next;
		free(cp);
		cp = client;
	} else {
		cl_before = client;
		cl_tmp = client->next;
		while (cl_tmp) {
			if (cl_tmp == cp) {
				cl_before->next = cp->next;
				free(cp);
				break;
			}
			cl_before = cl_tmp;
			cl_tmp = cl_tmp->next;
		}
		cp = cl_before->next;
	}
	client_num--;
	return cp;
}

void
communicate(void)
{

	signal(SIGPIPE, SIG_IGN);

	for (; ;) {
#ifdef SVR4
		int i;

		while (poll(poll_fds, maxfds, -1) == ERROR) {
			if (errno != EINTR)
				error_out("poll");
			errno = 0;
		}
#else

		fd_set	readfds;

		readfds = fds_org;
		while (select(maxfds, &readfds, NULL, NULL, NULL) == ERROR) {
			if (errno != EINTR)
				error_out("select");
			errno = 0;
		}
#endif
		
		for (cur_client = client; cur_client;) {
			if (setjmp(client_dead)) {
				cur_client = disconnect_client(cur_client);
				continue;
			}
#ifdef SVR4
			for (i = 0; i < maxfds; i++) {
				if ((poll_fds[i].fd == cur_client->fd) &&
				    (poll_fds[i].revents == POLLIN)) {
					poll_fds[i].revents = 0;
					client_fd = cur_client->fd;
					putpos = buflen = getpos = 0;
					execute_cmd();
					break;
				}
			}
			cur_client = cur_client->next;
		}
		connect_client(poll_fds);
#else
			if (FD_ISSET(cur_client->fd, &readfds)) {
				client_fd = cur_client->fd;
				putpos = buflen = getpos = 0;
				execute_cmd();
			}
			cur_client = cur_client->next;
		}
		connect_client(&readfds);
#endif
	}
}

void
put_flush(void)
{
	int	 i, len;
	char	*p;

	for (len = putpos, p = putbuf; len > 0;) {
		if ((i = write(client_fd, p, len)) == ERROR)
			longjmp(client_dead, -1);
		len -= i;
		p += i;
	}
	putpos = 0;
}

void
put_byte(int c)
{
	putbuf[putpos++] = c;
	if (putpos >= sizeof(putbuf))
		put_flush();
}

void
put_work(int c)
{
	put_byte(c >> 8);
	put_byte(c & 0xff);
}

void
put_int(int c)
{
	put_byte(c >> (8 * 3));
	put_byte(c >> (8 * 2));
	put_byte(c >> (8 * 1));
	put_byte(c);
}

u_char *
put_string(u_char *p)
{
	if (p) {
		do {
			put_byte(*p);
		} while (*p++);
	} else {
		put_byte(0);
	}
	return p;
}

u_char *
put_ndata(u_char *p, int n)
{
	while (n-- > 0)
		put_byte(p ? *p++ : 0);
	return p;
}

void
get_buf(void)
{
	int	i;

	for (;;) {
		if ((i = read(client_fd, getbuf, sizeof(getbuf))) > 0)
			break;
		if (i == ERROR) {
			if (errno == EINTR)
				continue;
			if (errno == EWOULDBLOCK)
				continue;
		}
		longjmp(client_dead, -1);
	}
	buflen = i;
	getpos = 0;
}

int
get_byte(void)
{
	if (getpos >= buflen)
		get_buf();
	return getbuf[getpos++] & 0xff;
}

int
get_word(void)
{
	int	i;

	i = get_byte();
	return (i << 8) | get_byte();
}

int
get_int(void)
{
	int	i0, i1, i2;

	i0 = get_byte();
	i1 = get_byte();
	i2 = get_byte();
	return (i0 << (8*3)) | (i1 << (8*2)) | (i2 << (8*1)) | get_byte();
}

int
get_nstring(u_char *p, int n)
{
	int	c;

	do {
		c = get_byte();
		if (n-- > 0)
			*p++ = c;
	} while (c);

	return (n < 0) ? ERROR : 0;
}

u_char *
get_ndata(u_char *p, int n)
{
	while (n-- > 0)
		*p++ = get_byte();
	return p;
}
