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

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <netinet/in.h>

#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "Const.h"
#include "sj3cmd.h"
#include "sj3err.h"
#include "sj3lib.h"

#ifdef SVR4
#define signal sigset
#endif

int	sj3_error_number;
char	*sj3_socket_name	= SocketName;
char	*sj3_port_name	= PortName;
int	sj3_port_number	= PortNumber;

#define BUFFER_SIZE BUFSIZ  
static	char	*af_unix_str = "unix";
static	u_char	putbuf[BUFFER_SIZE];
static	u_char	getbuf[BUFFER_SIZE];	
static	int	putpos = 0;		
static	int	getlen = 0;		
static	int	getpos = 0;		
static	SJ3_CLIENT_ENV	*cliptr;
static	int	server_fd = -1;

static        int     sj3_timeout = 0;                

static  int     INTLEN = sizeof(int);
static  int     CMDLEN = sizeof(int);
static  int     ReadErrorFlag = FALSE;

#define	client_init(p)	{ \
		cliptr = (p); \
		if ((server_fd = cliptr -> fd) == -1) { \
			sj3_error_number = SJ3_NotOpened; \
			return ERROR; \
		} \
	}

static int
put_flush(void)
{
	int	i, j;
	u_char	*p;

	for (i = putpos, p = putbuf ; i > 0 ; i -= j, p += j) {
	  if ((j = write(server_fd, p, i)) <= 0) {
	    shutdown(server_fd, 2);
	    close(server_fd);
	    cliptr -> fd = server_fd = -1;
	    sj3_error_number = SJ3_ServerDown;
	    return(ERROR);
	  }
	}
	putpos = 0;

	return 0;
}

static void
put_byte(int c)
{
	putbuf[putpos++] = c;
}

static void
put_word(int c)
{
	put_byte(c >> 8);
	put_byte(c & 0xff);
}

static void
put_int(int c)
{
	put_byte(c >> (8 * 3));
	put_byte(c >> (8 * 2));
	put_byte(c >> (8 * 1));
	put_byte(c);
}

static void
put_cmd(int cmd)
{
        ReadErrorFlag = FALSE;
	putpos = getlen = 0;
	put_int(cmd);
}

#define put_string put_ndata

static u_char*
put_ndata(u_char* p, int n)
{
	while (n-- > 0) put_byte(p ? *p++ : 0);
	return p;
}

static int
put_over(
	int	buflen,
	int	n,
	u_char* (*func1)(), u_char* str1, int len1,
	u_char* (*func2)(), u_char* str2, int len2,
	u_char* (*func3)(), u_char* str3, int len3,
	u_char* (*func4)(), u_char* str4, int len4)
{
#define ARGNUM 4
  u_char *(*func[ARGNUM])();
  u_char *data[ARGNUM];
  int len[ARGNUM];
  int i;

  func[0] = func1; data[0] = str1; len[0] = len1;
  func[1] = func2; data[1] = str2; len[1] = len2;
  func[2] = func3; data[2] = str3; len[2] = len3;
  func[3] = func4; data[3] = str4; len[3] = len4;

  for(i = 0; i < n; i++) {
    if (len[i] < buflen) {
      (*func[i])(data[i], len[i]);
      buflen -= len[i];
    } else {
      while (len[i] > 0) {
        data[i] = (*func[i])(data[i], (len[i] < buflen) ? len[i] : buflen);
	if (put_flush() == ERROR) return ERROR;
	len[i] -= buflen;
	buflen = BUFFER_SIZE;
      }
    }
  }
  if ((buflen != BUFFER_SIZE) && (put_flush() == ERROR))
    return ERROR;

  return  0;
}

static int
get_buffer(void)
{
        if (ReadErrorFlag) return ERROR;  
        getpos = getlen = 0;

        getlen = read(server_fd, getbuf, sizeof(getbuf));
        if (getlen <=0) {
	  shutdown(server_fd, 2);
	  close(server_fd);
	  cliptr -> fd = server_fd = -1;
	  sj3_error_number = SJ3_ServerDown;
	  return ERROR;
	}
        return getlen;
}

static u_char
get_byte(void)
{
        if ((getpos >= getlen) && (get_buffer() == ERROR)) {
	  ReadErrorFlag = TRUE;
	  return 0;
	}
	return(getbuf[getpos++] & 0xff);
}

static int
get_word(void)
{
	int	i;

	i = get_byte();
	return ((i << 8) | get_byte());
}

static int
get_int(void)
{
        int    i0;
        int    i1;
        int    i2;

	i0 = get_byte();
	i1 = get_byte();
	i2 = get_byte();
	return ((i0 << (8*3)) | (i1 << (8*2)) | (i2 << (8*1)) | get_byte());
}

static u_char*
get_string(u_char* p)
{
	int	c;

	do {
		*p++ = c = get_byte();
	} while (c);

	return p;
}

static int
get_nstring(u_char* p, int n)
{
	int	c;

	c = get_byte();
	while (c) {
		if (n > 1) { *p++ = c; n--; }
		c = get_byte();
	}
	if (n > 0) *p++ = 0;
	return n;
}

static u_char*
get_ndata(u_char* p, int n)
{
	while (n-- > 0) *p++ = get_byte();
	return p;
}

static void
skip_string(void)
{
	while (get_byte()) ;
}

static void
skip_ndata(int n)
{
	while (n-- > 0) get_byte();
}

static int
open_unix(void)
{
	int	fd, len;
	struct	sockaddr_un	sunix;

	sunix.sun_family      = AF_UNIX;
	strcpy(sunix.sun_path, sj3_socket_name);

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == ERROR) {
		sj3_error_number = SJ3_OpenSocket;
		return ERROR;
	}

	len = strlen(sunix.sun_path)+sizeof(sunix.sun_family);
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
	len += sizeof(sunix.sun_len);
#endif
	if (connect(fd, (struct sockaddr *)&sunix, len)
			== ERROR) {
		sj3_error_number = SJ3_ConnectSocket;
		return ERROR;
	}

	return fd;
}

void
sj3_set_timeout(int timeout)
{
	sj3_timeout = timeout;  
}

static void
connect_timeout(void)
{
}

static int
open_inet(char* host)
{
	struct	hostent	*hp;
	struct	servent	*sp;
	int	port;
	struct	sockaddr_in	sin;
	int	fd, ret;

	if (!(hp = gethostbyname(host))) {
		sj3_error_number = SJ3_GetHostByName;
		return ERROR;
	}

	if ((sp = getservbyname(sj3_port_name, "tcp")))
		port = ntohs(sp -> s_port);
	else
		port = sj3_port_number;

	memset((char *)&sin, '\0',sizeof(sin));
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(port);
	memcpy(&sin.sin_addr, hp -> h_addr, hp -> h_length);

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		sj3_error_number = SJ3_OpenSocket;
		return ERROR;
	}
        
        if (sj3_timeout > 0) {
                signal(SIGALRM, (void (*)())connect_timeout);
                alarm(sj3_timeout);
        }
        ret = connect(fd, (struct sockaddr *)&sin, sizeof(sin));
        if (sj3_timeout > 0) {
                alarm(0);
                signal(SIGALRM, SIG_IGN);
        }
        if (ret == ERROR) {
		sj3_error_number = SJ3_ConnectSocket;
		return ERROR;
	}

	return fd;
}


int
sj3_make_connection(
	SJ3_CLIENT_ENV*	client,
	char*	serv,
	char*	user,
	char*	prog)
{
	char	host[MAXHOSTNAMELEN];
	int	tmp;
        int     hostlen, userlen, proglen, datalen, buflen;
        int     curlen;
        char    *curdata;

	client -> fd = -1;

	if (!serv || *serv == '\0' || !strcmp(serv, af_unix_str))
	{
		if ((server_fd = open_unix()) == ERROR) return ERROR;
		strcpy(host, af_unix_str);
	}
	else {
		if ((server_fd = open_inet(serv)) == ERROR) return ERROR;
		gethostname(host, sizeof(host));
	}
	client -> fd = server_fd;
	client_init(client);

        hostlen = strlen(host) + 1;
        userlen = strlen(user) + 1;
        proglen = strlen(prog) + 1;
        datalen = hostlen + userlen + proglen;

	put_cmd(SJ3_CONNECT);
	put_int(SJ3SERV_VERSION_NO);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_string(host, hostlen);
	  put_string(user, userlen);
	  put_string(prog, proglen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, host, hostlen, put_string, user, 
		       userlen, put_string, prog, proglen, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

        if ((tmp = get_int()) == SJ3_DifferentVersion) {
		if (ReadErrorFlag) return ERROR;
		put_cmd(SJ3_CONNECT);
		put_int(1);
		if (datalen <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
			put_string(host, hostlen);
			put_string(user, userlen);
			put_string(prog, proglen);
			if (put_flush() == ERROR) return ERROR;
		} else {
			if (put_over(buflen, 3, put_string, host, hostlen, put_string, user, 
				     userlen, put_string, prog, proglen, NULL, NULL, 0) == ERROR)
			    return ERROR;
		}
		if ((tmp = get_int())) {
			sj3_erase_connection(client);
			sj3_error_number = tmp;
			return ERROR;
		}
	} else if (tmp && (-2 < tmp)) {
		sj3_erase_connection(client);
		sj3_error_number = tmp;
		return ERROR;
	}		
        cliptr -> svr_version = ((tmp)? -tmp : 1);
        cliptr -> cli_version = SJ3SERV_VERSION_NO;
        cliptr -> default_char[0] = 0x81;
        cliptr -> default_char[1] = 0x40;
	sj3_error_number = 0;

        if (ReadErrorFlag) return ERROR;
	put_cmd(SJ3_STDYSIZE);
        if (put_flush() == ERROR) return ERROR;

	if ((tmp = get_int())) {
		sj3_erase_connection(client);
		sj3_error_number = tmp;
		return ERROR;
	}

	cliptr -> stdy_size = get_int();
	cliptr -> fd        = server_fd;

        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_erase_connection(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_DISCONNECT);
        if (put_flush() == ERROR) return ERROR;

	sj3_error_number = get_int();
	close(server_fd);
	cliptr -> fd = -1;
        if (ReadErrorFlag) return ERROR;
	return sj3_error_number ? ERROR : 0;
}


long
sj3_open_dictionary(
	SJ3_CLIENT_ENV* client,
	char* dictname, char* password)
{
        int res;
        int dictlen, passlen;
        int datalen, buflen;

	client_init(client);

        dictlen = strlen(dictname) + 1;
        passlen = ((password) ? strlen(password) : 0) + 1;
        datalen = dictlen + passlen;

	put_cmd(SJ3_OPENDICT);
        if (datalen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(dictname, dictlen);
	  put_string(password, passlen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 2, put_string, dictname, dictlen, put_string, 
		       password, passlen, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if ((sj3_error_number = get_int())) return 0;
        res = get_int();
        return ReadErrorFlag ? ERROR : res;
}

int
sj3_close_dictionary(SJ3_CLIENT_ENV* client, long dicid)
{
	client_init(client);

	put_cmd(SJ3_CLOSEDICT);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_open_study_file(
	SJ3_CLIENT_ENV* client,
	char* stdyname, char* password)
{
        int stdylen, passlen;
        int datalen, buflen;

	client_init(client);

        stdylen = strlen(stdyname) + 1;
        passlen = strlen(password) + 1;
        datalen = stdylen + passlen;

	put_cmd(SJ3_OPENSTDY);
        if (datalen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(stdyname, stdylen);
	  put_string(password, passlen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 2, put_string, stdyname, stdylen, put_string, 
		       password, passlen, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_close_study_file(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_CLOSESTDY);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_get_id_size(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_STDYSIZE);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
	cliptr -> stdy_size = get_int();
        return ReadErrorFlag ? ERROR : cliptr -> stdy_size;
}


int
sj3_lock_server(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_LOCK);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_unlock_server(SJ3_CLIENT_ENV *client)
{
	client_init(client);

	put_cmd(SJ3_UNLOCK);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_ikkatu_henkan(
	SJ3_CLIENT_ENV*	client,
	u_char*	src,
	u_char*	dst,
	int	dstsiz,
	int	mb_flag)
{
	int	c;
	u_char	*top;
	int	result;
	int	len;
	int	len1;
	int	stysiz;

        int     srclen;
        int     buflen;

	client_init(client);

        srclen = strlen(src) + 1;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PH2KNJ);
	else
	  put_cmd(SJ3_PH2KNJ_EUC);
        if (srclen < (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(src, srclen);
	  if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 1, put_string, src, srclen, NULL, NULL, 0, 
			  NULL, NULL, 0, NULL, NULL, 0) == ERROR)
	    return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;

	result = get_int();

	
	result = 0;
	stysiz = cliptr -> stdy_size;
	len1 =  1 + stysiz + 1 + 1;	
	while ((c = get_byte())) {

		top = dst;		
		if (dstsiz < len1) goto error1;

		*dst++ = len = c;
		dst = get_ndata(dst, stysiz);
		dstsiz -= (stysiz + 1);

		while ((c = get_byte())) {
			if (dstsiz-- < 3) goto error2;
			*dst++ = c;
		}
		*dst++ = 0;
		dstsiz--;
		result += len;
	}

	*dst = 0;
        return ReadErrorFlag ? ERROR : result;

	
error1:
	do {
		skip_ndata(stysiz);
error2:
		while (get_byte()) ;
	} while (get_byte());

	*top = 0;
        return ReadErrorFlag ? ERROR : result;
}


int
sj3_bunsetu_henkan(
	SJ3_CLIENT_ENV*	client,
	u_char*	yomi,
	int	len,
	u_char*	kanji,
	int	mb_flag)
{
	int	result;
        int     buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ);
	else
	  put_cmd(SJ3_CL2KNJ_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  
	  if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
		       NULL, NULL, 0, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}

int
sj3_bunsetu_jikouho(
	SJ3_CLIENT_ENV*	client,
	u_char*	kanji,
	int	mode,
	int	mb_flag)
{
	int	result;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_NEXTCL);
	else
	  put_cmd(SJ3_NEXTCL_EUC);
	put_int(mode);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}

int
sj3_bunsetu_maekouho(
	SJ3_CLIENT_ENV* client,
	u_char* kanji, int mode, int mb_flag)
{
	int	result;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PREVCL);
	else
	  put_cmd(SJ3_PREVCL_EUC);
	put_int(mode);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;

	result = get_int();
	get_string( get_ndata(kanji, cliptr -> stdy_size) );
        return ReadErrorFlag ? ERROR : result;
}


int
sj3_bunsetu_kouhosuu(
	SJ3_CLIENT_ENV*	client,
	u_char*	yomi,
	int	len,
	int	mb_flag)
{
        int result;
        int buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ_CNT);
	else
	  put_cmd(SJ3_CL2KNJ_CNT_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	
          if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
                       NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;

	result = get_int();
        return ReadErrorFlag ? ERROR : result;
}

int
sj3_bunsetu_zenkouho(
	SJ3_CLIENT_ENV*	client,
	u_char*	yomi,
	int	len,
	SJ3_DOUON*	douon,
	int	mb_flag)
{
	int	cnt = 0;
        int buflen;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CL2KNJ_ALL);
	else
	  put_cmd(SJ3_CL2KNJ_ALL_EUC);
	put_int(len);
        if ((len + 1) <= (buflen = BUFFER_SIZE - CMDLEN - INTLEN)) {
	  put_ndata(yomi, len);
	  put_byte(0);
          if (put_flush() == ERROR) return ERROR;
	} else {
	
          if (put_over(buflen, 2, put_ndata, yomi, len, put_ndata, NULL, 1,
                       NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;

	while (get_int()) {
                get_ndata(&(douon -> dcid), cliptr -> stdy_size);
		get_string(douon -> ddata);
		douon -> dlen = strlen(douon -> ddata);
		douon++;
		cnt++;
	}

        return ReadErrorFlag ? ERROR : cnt;
}


int
sj3_tango_gakusyuu(SJ3_CLIENT_ENV* client, SJ3_STUDYREC* stdy)
{
        int buflen;

	client_init(client);
    
	put_cmd(SJ3_STUDY);
        if (cliptr -> stdy_size <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_ndata(stdy, cliptr -> stdy_size);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  
	  if (put_over(buflen, 1, put_ndata, stdy, cliptr -> stdy_size, NULL, 
		       NULL, 0, NULL, NULL, 0, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}


	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_bunsetu_gakusyuu(
	SJ3_CLIENT_ENV*	client,
	u_char*	yomi1,
	u_char*	yomi2,
	SJ3_STUDYREC*	stdy,
	int	mb_flag)
{
        int yomilen1, yomilen2;
        int datalen, buflen;

	client_init(client);

        yomilen1 = strlen(yomi1) + 1;
        yomilen2 = strlen(yomi2) + 1;
        datalen = yomilen1 + yomilen2 + cliptr -> stdy_size;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_CLSTUDY);
	else
  	  put_cmd(SJ3_CLSTUDY_EUC);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(yomi1, yomilen1);
	  put_string(yomi2, yomilen2);
	  put_ndata(stdy, cliptr -> stdy_size);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi1, yomilen1, put_string, 
		       yomi2, yomilen2, put_ndata, stdy, cliptr->stdy_size,
		       NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_tango_touroku(
	SJ3_CLIENT_ENV* client,
	long	dicid,
	u_char*	yomi,
	u_char*	kanji,
	int	code,
	int	mb_flag)
{
        int yomilen, kanjilen;
        int datalen, buflen;

	client_init(client);

        yomilen = strlen(yomi) + 1;
        kanjilen = strlen(kanji) + 1;
        datalen = yomilen + kanjilen + INTLEN;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_ADDDICT);
	else
	  put_cmd(SJ3_ADDDICT_EUC);	  
	put_int(dicid);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN -INTLEN)) {
	  put_string(yomi, yomilen);
	  put_string(kanji, kanjilen);
	  put_int(code);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi, yomilen, put_string, kanji,
		       kanjilen, put_ndata, &code, INTLEN, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_tango_sakujo(
	SJ3_CLIENT_ENV*	client,
	long	dicid,
	u_char*	yomi,
	u_char*	kanji,
	int	code,
	int	mb_flag)
{
        int yomilen, kanjilen;
        int datalen, buflen;

	client_init(client);

        yomilen = strlen(yomi) + 1;
        kanjilen = strlen(kanji) + 1;
        datalen = yomilen + kanjilen + INTLEN;

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_DELDICT);
	else
	  put_cmd(SJ3_DELDICT_EUC);
	put_int(dicid);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN -INTLEN)) {
	  put_string(yomi, yomilen);
	  put_string(kanji, kanjilen);
	  put_int(code);
          if (put_flush() == ERROR) return ERROR;
	} else {
	  if (put_over(buflen, 3, put_string, yomi, yomilen, put_string, kanji,
		       kanjilen, put_ndata, &code, INTLEN, NULL, NULL, 0) == ERROR) 
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_tango_syutoku(
	SJ3_CLIENT_ENV*	client,
	int	dicid,
	u_char*	buf,
	int	mb_flag)
{
	u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_GETDICT);
	else
	  put_cmd(SJ3_GETDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_tango_jikouho(
	SJ3_CLIENT_ENV*	client,
	int	dicid,
	u_char*	buf,
	int	mb_flag)
{
	u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_NEXTDICT);
	else
	  put_cmd(SJ3_NEXTDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_tango_maekouho(
	SJ3_CLIENT_ENV*	client,
	int	dicid,
	u_char*	buf,
	int	mb_flag)
{
	u_char	*p;

	client_init(client);

	if (mb_flag == MBCODE_SJIS)
	  put_cmd(SJ3_PREVDICT);
	else
	  put_cmd(SJ3_PREVDICT_EUC);
	put_int(dicid);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;

	p = get_string(buf);
	p = get_string(p);
	*p = get_int();
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_make_dict_file(
	SJ3_CLIENT_ENV*	client,
	char*	path,
	int	idxlen,
	int	seglen,
	int	segnum)
{
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN + INTLEN + INTLEN;

	put_cmd(SJ3_MAKEDICT);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(idxlen);
	  put_int(seglen);
	  put_int(segnum);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 4, put_string, path, pathlen, put_ndata,
                       &idxlen, INTLEN, put_ndata, &seglen, INTLEN,
                       put_ndata, &segnum, 0) == ERROR)
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_make_study_file(
	SJ3_CLIENT_ENV*	client,
	char*	path,
	int	stynum,
	int	clstep,
	int	cllen)
{
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN + INTLEN + INTLEN;

	put_cmd(SJ3_MAKESTDY);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(stynum);
	  put_int(clstep);
	  put_int(cllen);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 4, put_string, path, pathlen, put_ndata,
                       &stynum, INTLEN, put_ndata, &clstep, INTLEN,
                       put_ndata, &cllen, 0) == ERROR)
            return ERROR;
	}

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_make_directory(SJ3_CLIENT_ENV* client, char* path)
{
        int pathlen;
        int buflen;

	client_init(client);

        pathlen = strlen(path) + 1;

	put_cmd(SJ3_MAKEDIR);
        if (pathlen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 1, put_string, path, pathlen, NULL, NULL, 0,
		       NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}
	  

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_access(SJ3_CLIENT_ENV* client, char* path, int mode)
{
        int result;
        int pathlen;
        int buflen, datalen;

	client_init(client);

        pathlen = strlen(path) + 1;
        datalen = pathlen + INTLEN;

	put_cmd(SJ3_ACCESS);
        if (datalen <= (buflen = BUFFER_SIZE - CMDLEN)) {
	  put_string(path, pathlen);
	  put_int(mode);
          if (put_flush() == ERROR) return ERROR;
	} else {
          if (put_over(buflen, 2, put_string, path, pathlen, put_ndata,
                       &mode, INTLEN, NULL, NULL, 0, NULL, NULL, 0) == ERROR)
            return ERROR;
	}

	sj3_error_number = 0;
	result = get_int();
        return ReadErrorFlag ? ERROR : result;
}


int
sj3_who(SJ3_CLIENT_ENV* client, SJ3_WHO_STRUCT* ret, int num)
{
	int	i, j;

	client_init(client);

	put_cmd(SJ3_WHO);
        if (put_flush() == ERROR) return ERROR;

	if ((i = get_int()) < 0) {
		sj3_error_number = SJ3_InternalError;
		return -1;
	}

	sj3_error_number = 0;
	for (j = 0 ; j < i ; j++) {
		if (j < num) {
			ret -> fd = get_int();
			get_nstring(ret -> hostname, SJ3_NAME_LENGTH);
			get_nstring(ret -> username, SJ3_NAME_LENGTH);
			get_nstring(ret -> progname, SJ3_NAME_LENGTH);
			ret++;
		}
		else {
			get_int();
			skip_string();
			skip_string();
			skip_string();
		}
	}

        return ReadErrorFlag ? ERROR : ((i < num) ? i : num);
}


int
sj3_quit(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_QUIT);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}

int
sj3_kill(SJ3_CLIENT_ENV* client)
{
	client_init(client);

	put_cmd(SJ3_KILL);
        if (put_flush() == ERROR) return ERROR;

	if ((sj3_error_number = get_int())) return ERROR;
        return ReadErrorFlag ? ERROR : 0;
}


int
sj3_version(SJ3_CLIENT_ENV* client, char* dst, int dstsiz)
{
	int	c;

	client_init(client);

	put_cmd(SJ3_VERSION);
        if (put_flush() == ERROR) return ERROR;

	sj3_error_number = get_int();
	c = get_byte();
	while (c) {
		while (c) {
			if (dstsiz > 2) { *dst++ = c; dstsiz--; }
			c = get_byte();
		}
		if (dstsiz > 1) { *dst++ = 0; dstsiz--; }
		c = get_byte();
	}
	if (dstsiz > 0) { *dst++ = 0; dstsiz--; }
        return ReadErrorFlag ? ERROR : 0;
}
