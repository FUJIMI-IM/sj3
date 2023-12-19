/*
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
 *
 */

/*
 * $SonyRCSfile: comuni.c,v $  
 * $SonyRevision: 1.4 $ 
 * $SonyDate: 1994/11/16 07:34:05 $
 *
 * $Id$
 */


#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "sj_sysvdef.h"
#include "sj_kcnv.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include "server.h"
#include "kanakan.h"
#ifdef SVR4
#include <sys/filio.h>
#include <sys/signal.h>
#include <poll.h>
#endif
#ifdef TLI
#include <fcntl.h>
#include <tiuser.h>
#include <stropts.h>   
#include <sys/types.h>
#include <netdir.h>
#include <netconfig.h>
#endif

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

int	client_num = 0;		
Client	*client = NULL;
Client  *cur_client;

int	maxfds = 0;		
#ifdef SVR4
struct pollfd poll_fds[_NFILE];
#else
fd_set	fds_org = { 0 };	
#endif
jmp_buf	client_dead;
int	client_fd;

extern	char	*socket_name;
extern	char	*port_name;
#ifdef TLI
extern  char    *port_number;
extern  char    *proto_name;
#else
extern	int	port_number;
#endif
extern	int	max_client;

static	int	fd_inet = -1;
static	int	fd_unix = -1;

static	char	putbuf[BUFSIZ];		
static	char	getbuf[BUFSIZ];		
static	int	putpos = 0;		
static	int	buflen = 0;		
static	int	getpos = 0;		


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
	if (fd >= maxfds) maxfds = fd + 1;
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
				poll_fds[j].fd = poll_fds[j+1].fd;
				poll_fds[j].events = poll_fds[j+1].events;
				poll_fds[j].revents = poll_fds[j+1].revents; 
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
			if (FD_ISSET(fd, &fds_org)) break;
		}
		maxfds = fd + 1;
	}
#endif
}

#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))


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

	if (bind(fd_unix, (struct sockaddr *)&sunix, strlen(sunix.sun_path) + 2) == ERROR) {
		if (errno == EADDRINUSE)
			fprintf(stderr, "Port '%s' is already in use\n",
				socket_name);
		else
			fprintf(stderr, "Can't bind AF_UNIX socket\n");
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
#endif

#ifdef TLI
struct t_call *inet_callptr;
struct netconfig *nconf;
#endif


static void
open_af_inet(void)
{
#ifdef TLI
        struct t_bind *req;
        struct nd_hostserv hserv;
        struct nd_addrlist *addrs;  
	struct t_optmgmt *opt;
	struct opthdr *hdr;
        void *handlep;
        extern int t_errno;
        extern void *setnetpath(void);
        extern struct netconfig *getnetpath(void *);
#else
	struct sockaddr_in	sin;
	struct servent	*sp;
	unsigned short	port;
#endif
	int	true = 1;

	
#ifdef TLI
	  if (( handlep = setnetpath()) == NULL) {
                fprintf(stderr, "Setnetpath fail\n");
                exit(1);
	  }

	while((nconf = getnetpath(handlep)) != NULL) {
                if ((nconf->nc_semantics == NC_TPI_COTS_ORD) &&
                    (strncmp(nconf->nc_proto, proto_name, strlen(proto_name)) == 0))
                  break;
	}
	if (nconf == NULL) {
                fprintf(stderr, "No connection mode transport\n");
                exit(1); 
	}
        hserv.h_host = HOST_SELF;
        hserv.h_serv = port_number;
	if (netdir_getbyname(nconf, &hserv, &addrs) != 0) {
                hserv.h_serv = port_name;
		if (netdir_getbyname(nconf, &hserv, &addrs) != 0) {
                      fprintf(stderr, "Can't get address\n");
                      exit(1);
		}
	}
	if ((fd_inet = t_open(nconf->nc_device, O_RDWR, (struct t_info *) NULL)) < 0) {
                fprintf(stderr,"Can't t_open\n");
                exit(1);
	}

	if ((opt = (struct t_optmgmt *) t_alloc(fd_inet, T_OPTMGMT, T_ALL)) == NULL) {
		fprintf(stderr, "Can't t_alloc error for T_OPTMGT\n");
		exit(1);
	}
	if ((hdr = (struct opthdr *) malloc(sizeof(*hdr) + sizeof(true))) == NULL) {
		fprintf(stderr, "server: not malloc\n");
		exit(1);
	}
        hdr->level = SOL_SOCKET;
        hdr->name  = SO_REUSEADDR;
        hdr->len   = sizeof(true);
      
        (void) memcpy(&true, (char *)hdr + sizeof(*hdr), sizeof(true));

	opt->opt.len  = opt->opt.maxlen = sizeof(*hdr) + sizeof(true);
        opt->flags    = T_CHECK;
        opt->opt.buf  = (char *)hdr;

	if ((t_optmgmt(fd_inet, opt, opt) < 0) || !(opt->flags & T_SUCCESS)){
		fprintf(stderr, "Warning: Can't reuse address %s(%s)\n",
			port_name, port_number);
	} else {
		opt->flags    = T_NEGOTIATE;		
	      
		(void) memcpy(&true, (char *)hdr + sizeof(*hdr), sizeof(true));
		(void) t_optmgmt(fd_inet, opt, opt);
	}

        if ((req = (struct t_bind *) t_alloc(fd_inet, T_BIND, T_ALL)) == NULL) {
                t_close(fd_inet);
                fprintf(stderr, "Can't alloc bind data\n");
                exit(1);
	}
        req->addr.maxlen = addrs->n_addrs->maxlen;
        req->addr.len = addrs->n_addrs->len;
      
        (void) memcpy(addrs->n_addrs->buf, req->addr.buf, addrs->n_addrs->len);
	req->qlen = 5;   
	
	if (t_bind(fd_inet, req, req) < 0 ) {
		fprintf(stderr, "Can't t_bind\n");
		t_close(fd_inet);
		exit(1);
	}
	if (memcmp(req->addr.buf, addrs->n_addrs->buf, addrs->n_addrs->len) != 0) {
                fprintf(stderr, "Port '%s(%s)' is already in use\n",
                        port_name, port_number);
                t_close(fd_inet);
                exit(1);
	}
	if ((inet_callptr = (struct t_call *) t_alloc(fd_inet, T_CALL, T_ALL)) == NULL) {
                fprintf(stderr,"t_alloc error for T_CALL\n");
                t_close(fd_inet);
                exit(1);
	}

	inet_callptr->addr = *(addrs->n_addrs);


	inet_callptr->udata.maxlen = 0;
	inet_callptr->udata.len = 0;
	inet_callptr->udata.buf = (char *) NULL;
#else
	if ((sp = getservbyname(port_name, "tcp")) != NULL)
		port = ntohs(sp -> s_port);
	else
		port = port_number;

	memset((char *)&sin, '\0', sizeof(sin));
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((fd_inet = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		fprintf(stderr, "Can't create AF_INET socket\n");
		exit(1);
	}

	setsockopt(fd_inet, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(true));

	if (bind(fd_inet, (struct sockaddr *)&sin, sizeof(sin)) == ERROR) {
		if (errno == EADDRINUSE) {
			if (sp)
				fprintf(stderr,
					"Port '%s(%d)' is already in use\n",
					port_name, port);
			else
				fprintf(stderr,
					"Port %d is already in use\n",
					port);
		}
		else
			fprintf(stderr, "Can't bind AF_INET socket\n");
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
#endif
	set_fd(fd_inet);
}

void
open_socket(void)
{
	open_af_inet();
#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))
	open_af_unix();
#endif
}

#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))


static int
connect_af_unix(void)
{
	struct sockaddr_un	sunix;
	int	i = sizeof(sunix);
	int	fd;

	if ((fd = accept(fd_unix, (struct sockaddr *)&sunix, &i)) == ERROR)
		warning_out("Can't accept AF_UNIX socket");
	return fd;
}
#endif

#ifdef TLI
int
accept_func(int fd, struct t_call* callptr, char* name, int closeflg)
{
        int newfd;  
        int look;
        extern int t_errno;
                
	if ((newfd = t_open(name, O_RDWR, (struct t_info *)0)) < 0) {
                if (!closeflg) warning_out("Can't t_open");
                return ERROR;
	}
	if (t_bind(newfd, (struct t_bind *)0, (struct t_bind *)0) < 0) {
                if (!closeflg) warning_out("t_bind error");
                return ERROR;
	}
        t_errno = 0;
	if (t_accept(fd, newfd, callptr) <0 ){
                t_close(newfd);
                if (!closeflg) warning_out("t_accept error");
                return ERROR;
	}
	if (ioctl(newfd, I_POP, (char *) 0) < 0) {
                if (!closeflg) warning_out("I_POP timod failed");
                return ERROR;
	}
	if (ioctl(newfd, I_PUSH, "tirdwr") < 0) {
                if (!closeflg) warning_out("I_PUSH tirdwr failed");
                return ERROR;
	}
        return(newfd);
}
#endif



static int
connect_af_inet(void)
{
#ifndef TLI
	struct sockaddr_in	sin;
	int	i = sizeof(sin);
#endif
	int	fd;

#ifdef TLI
	  if (t_listen(fd_inet, inet_callptr) < 0) {
                warning_out("t_listen error");

	  }
        
        if ((fd = accept_func(fd_inet, inet_callptr, nconf->nc_device, FALSE)) == ERROR)
#else
	if ((fd = accept(fd_inet, (struct sockaddr *)&sin, &i)) == ERROR)
#endif
		warning_out("Can't accept AF_INET socket");
	return fd;
}

#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))


static void
close_af_unix(void)
{
	struct sockaddr_un	sunix;
	int	true = 1;
	int	i;

	ioctl(fd_unix, FIONBIO, &true);
	for ( ; ; ) {
		i = sizeof(sunix);
		if (accept(fd_unix, (struct sockaddr *)&sunix, &i) < 0) break;
	}
	shutdown(fd_unix, 2);
	close(fd_unix);
	unlink(socket_name);
	clr_fd(fd_unix);
}
#endif



static void
close_af_inet(void)
{
#ifndef TLI
	struct sockaddr_in	sin;
#endif
	int	true = 1;
	int	i;

	ioctl(fd_inet, FIONBIO, &true);
	for ( ; ; ) {
#ifdef TLI
                if (accept_func(fd_inet, inet_callptr, nconf->nc_device, TRUE) < 0) 
			break;
#else
		i = sizeof(sin);
		if (accept(fd_inet, (struct sockaddr *)&sin, &i) < 0) break;
#endif
	}
#ifdef TLI
	t_sndrel(fd_inet);
        t_close(fd_inet);
        clr_fd(fd_inet);
#else
	shutdown(fd_inet, 2);
	close(fd_inet);
	clr_fd(fd_inet);
#endif
}

void
close_socket(void)
{
#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))
	close_af_unix();
#endif
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
#if (defined(TLI) && defined(SOCK_UNIX))
	int     unix_flag=0;
#endif
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
#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))
		if ((readfds[i].fd == fd_unix) &&
		    (readfds[i].revents == POLLIN)) {
			fd = connect_af_unix();
			readfds[i].revents = 0;
#if (defined(TLI) && defined(SOCK_UNIX))
			unix_flag = 1;
#endif
			break;
		}
#endif
	}
	if (i == maxfds) return;
#else /* SVR4 */
	if (FD_ISSET(fd_inet, readfds))
		fd = connect_af_inet();
#if (!defined(TLI) || (defined(TLI) && defined(SOCK_UNIX)))
	else if (FD_ISSET(fd_unix, readfds))
#if (defined(TLI) && defined(SOCK_UNIX))
        {
		fd = connect_af_unix();
		unix_flag = 1;
	}
#else
		fd = connect_af_unix();
#endif
#endif
	else
		return;
#endif /* SVR4 */
	if (fd == ERROR) return;

	

	
	if (client_num >= MaxClientNum || client_num >= max_client) {
		warning_out("No more client");
	}

	else {
		debug_out(1, "client create(%d)\r\n", fd);
		logging_out("connect to client on %d", fd);

		
		if ((client_tmp = (Client *)malloc(sizeof(Client))) != NULL) {
			memset(client_tmp, 0, sizeof(Client));
			client_tmp->fd = fd;
#if (defined(TLI) && defined(SOCK_UNIX))
			client_tmp->unix_flag = unix_flag;
#endif
			client_tmp->next = client;
			client = client_tmp;
			client_num++;
			
			set_fd(fd);
			
			return;
		}
		warning_out("No more client(Can't alloc)");
	}
#ifdef TLI
#ifdef SOCK_UNIX
	if (unix_flag) {
		shutdown(fd, 2);
		close(fd);
	} else {
		t_sndrel(fd);
		t_close(fd);
	}
#else
	t_sndrel(fd);
	t_close(fd);
#endif
#else
	shutdown(fd, 2);
	close(fd);
#endif

}



static Client*
disconnect_client(Client *cp)
{
	WorkArea *wp;
	StdyFile *sp;
	int	fd;
	Client  *cl_tmp, *cl_before;

	if ((sp = cp->stdy) != NULL)  closestdy(sp);
	if ((wp = cp->work) != NULL)  free_workarea(wp);
  
	debug_out(1, "client dead(%d)\r\n", cp->fd);
	logging_out("disconnect from client on %d", cp->fd);
#ifdef TLI
#ifdef SOCK_UNIX
        if (cp->unix_flag) {
		shutdown(fd = cp->fd, 2);
		close(fd);
		clr_fd(fd);
	} else	{	
		fd = cp->fd;
		t_sndrel(fd);
		t_close(fd);
		clr_fd(fd);
	}
#else
        fd = cp->fd;
	t_sndrel(fd);
        t_close(fd);
        clr_fd(fd);
#endif
#else 
	shutdown(fd = cp->fd, 2);
	close(fd);
	clr_fd(fd);
#endif
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

#ifndef SIGPIPE
#include <signal.h>
#endif

void
communicate(void)
{
	
	signal(SIGPIPE, SIG_IGN);
        
	for ( ; ; ) {
#ifdef SVR4
		int i;

		while(poll(poll_fds, maxfds, -1) == ERROR) { 
			if (errno != EINTR) error_out("poll");
			errno = 0;
		}
#else

		fd_set	readfds;

		
		readfds = fds_org;
		while (select(maxfds,&readfds,NULL,NULL,NULL) == ERROR) {
			if (errno != EINTR) error_out("select");
			errno = 0;
		}
#endif
		
		for (cur_client = client ; cur_client; ) {

			if (setjmp(client_dead)) {
				cur_client = disconnect_client(cur_client);
				continue;
			}
#ifdef SVR4
			for ( i = 0; i < maxfds; i++) {
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
	int	len;
	int	i;
	char	*p;

	for (len = putpos, p = putbuf ; len > 0 ; ) {
		if ((i = write(client_fd, p, len)) == ERROR) {
			longjmp(client_dead, -1);
		}
		len -= i;
		p += i;
	}
	putpos = 0;
}


void
put_byte(int c)
{
	putbuf[putpos++] = c;
	if (putpos >= sizeof(putbuf)) put_flush();
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


unsigned char*
put_string(unsigned char* p)
{
	if (p) {
		do {
			put_byte(*p);
		} while (*p++);
	}
	else
		put_byte(0);
	return p;
}


unsigned char*
put_ndata(unsigned char* p, int n)
{
	while (n-- > 0) put_byte(p ? *p++ : 0);
	return p;
}


void
get_buf(void)
{
	int	i;

	for (;;) {
		if ((i = read(client_fd, getbuf, sizeof(getbuf))) > 0) break;
		if (i == ERROR) {
			if (errno == EINTR) continue;
			if (errno == EWOULDBLOCK) continue;
		}
		longjmp(client_dead, -1);
	}
	buflen = i;
	getpos = 0;
}


int
get_byte(void)
{
	if (getpos >= buflen) get_buf();
	return (getbuf[getpos++] & 0xff);
}


int
get_word(void)
{
	int	i;

	i = get_byte();
	return ((i << 8) | get_byte());
}


int
get_int(void)
{
	int	i0;
	int	i1;
	int	i2;

	i0 = get_byte();
	i1 = get_byte();
	i2 = get_byte();
	return((i0 << (8*3)) | (i1 << (8*2)) | (i2 << (8*1)) | get_byte());
}


int
get_nstring(unsigned char* p, int n)
{
	int	c;

	do {
		c = get_byte();
		if (n-- > 0) *p++ = c;
	} while (c);

	return (n < 0) ? ERROR : 0;
}


void*
get_ndata(void * p, int n)
{
	unsigned char *pp = (unsigned char *)p;
	while (n-- > 0) *pp++ = get_byte();
	return pp;
}
