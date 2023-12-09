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
 * $SonyRCSfile: sjgetchar.c,v $  
 * $SonyRevision: 1.5 $ 
 * $SonyDate: 1997/01/23 11:26:31 $
 */



#include "sj_sysvdef.h"
#include "wchar16.h"
#include "kctype.h"
#ifdef SVR4
#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#endif
#ifdef linux
#include <sys/types.h>
#include <sys/fcntl.h>
#endif
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define	USE_OLD_TTY
#include <curses.h>
#endif

#include <errno.h>
#include <sys/file.h>
#include "sj2.h"
#include "select.h"

#if !defined(FREAD) && defined(O_RDONLY)
#define		FREAD		(O_RDONLY+1)
#define		FWRITE		(O_WRONLY+1)
#endif

#define       Isknj1(s)       ((eucmode) ? (iseuc(s) || (SS2 == (s))) : (issjis1(s)))
#define       Isknj2(s)       ((eucmode) ? (SS3 == (s)) : 0)

static wchar16_t  backup = (wchar16_t) 0;

static int    eucmode = 0;
extern int	master;

wchar16_t SJ_getchar()
{
	SELECT_FD	ifds;
	wchar16_t		c;

	if (backup) {
		if (SJ_read (&c, 1) > 0)
			return (c);
	}
	for (;;) {
		FD_ZERO (&ifds);
		FD_SET (STDIN, &ifds);
		FD_SET (master, &ifds);
		if (select (master + 1, &ifds, 0, 0, 0) <= 0) {
			if (errno == EINTR) {
				errno = 0;
				continue;
			} else
				return (EOF);
		}

		if (FD_ISSET (STDIN, &ifds)) {
			if (SJ_read (&c, 1) > 0) {
				if (FD_ISSET (master, &ifds)) {
					output_master();
				}
				return (c);
			}
			return (EOF);
		}
		if (FD_ISSET (master, &ifds)) {
			output_master();
		}
	}
}

#ifdef SVR4
#define TIOCPKT_DOSTOP 0x10		
#define TIOCPKT_NOSTOP 0x20		

static int stopmode = TIOCPKT_DOSTOP;
static char stopc = CTRL('s');
static char startc = CTRL('q');
static char outbuf[BUFSIZ];
extern int slave;
extern char *slave_name, *ptsname();

output_master()
{
	struct strbuf ctlbuf;
	struct strbuf databuf;
	int flags = 0;
	u_char ctlstuff;
	int f, stop, ixon, ret;
	struct iocblk *ioc;
	struct termios *tp;
	struct termio *ti;
	struct tchars	tc, tc1;

	memset((char *)&ctlbuf, '\0', sizeof(ctlbuf));
	memset((char *)&databuf,'\0', sizeof(databuf));
	ctlbuf.maxlen = sizeof(ctlstuff);
	ctlbuf.buf = (char *)&ctlstuff;
	databuf.maxlen = BUFSIZ;
	databuf.buf = outbuf;
	if ((ret = getmsg(master, &ctlbuf, &databuf, &flags)) == -1) {
		return;
	}
        if (ctlbuf.len <= 0) {
		errno = EWOULDBLOCK;
		return;
	}
	if (ret & MORECTL) {
		errno = EWOULDBLOCK;
		return;
	}
	if ((ctlstuff != M_DATA) && (ret & MOREDATA)) {
		errno = EWOULDBLOCK;
		return;
	}
	if (ctlbuf.len != sizeof(ctlstuff)) {
		errno = EWOULDBLOCK;
		return;
	}
	switch (ctlstuff) {
	case M_DATA:
	
		write_stdout((u_char *)&outbuf[0], databuf.len);
		fflush (stdout);
		while (ret & MOREDATA) {
			if ((ret = getmsg(master, &ctlbuf, &databuf, &flags)) == -1) {
				  return;
			}
			write_stdout((u_char *)&outbuf[0], databuf.len);
			fflush (stdout);
		}
		return;
		break;

	case M_FLUSH:
		if (databuf.len <= 0) {
			errno = EWOULDBLOCK;
			return;
		}
		fflush (stdout);
		break;

	case M_IOCTL:
		ioc = (struct iocblk *)databuf.buf;
		if (databuf.len <= sizeof(struct iocblk)) {
			errno = EWOULDBLOCK;
			return;
		}
		switch (ioc->ioc_cmd) {
		case TCSETS:
		case TCSETSW:
		case TCSETSF:
			if (databuf.len < (sizeof(struct termios)
						+ sizeof(struct iocblk))) {
				errno = EWOULDBLOCK;
				return;
			}
			tp = (struct termios *)(databuf.buf + sizeof (struct iocblk));
			stopc = tp->c_cc[VSTOP];
			startc = tp->c_cc[VSTART];
			ixon = tp->c_iflag & IXON;
			break;
		case TCSETA:
		case TCSETAW:
		case TCSETAF:
			if (databuf.len < (sizeof (struct termio)
					+ sizeof (struct iocblk))) {
				errno = EWOULDBLOCK;
				return;
			}
			ti = (struct termio *)(databuf.buf + sizeof (struct iocblk));
			ixon = ti->c_iflag & IXON;
			break;

		default:
			errno = EWOULDBLOCK;
			return;
		}
		stop = (ixon && (stopc == CTRL('s')) && (startc == CTRL('q')));
		if (stopmode == TIOCPKT_NOSTOP) {
			if (stop) {
				ioctl (STDOUT, TIOCGETC, &tc);
				

				if (slave == 0) {
					slave_name = ptsname(master); 
					slave = open(slave_name, O_RDWR);
				}
				ioctl (slave, TIOCGETC, &tc1);
				tc.t_stopc = tc1.t_stopc;
				tc.t_startc = tc1.t_startc;
				ioctl (STDOUT, TIOCSETC, &tc);
				stopmode = TIOCPKT_DOSTOP;
			}
		} else {
			if (!stop) {
				ioctl (STDOUT, TIOCGETC, &tc);
				tc.t_stopc = -1;
				tc.t_startc = -1;
				ioctl (STDOUT, TIOCSETC, &tc);
				stopmode = TIOCPKT_NOSTOP;
			}
		}
		errno = EWOULDBLOCK;
		return;
		break;
	default:
		
		errno = EWOULDBLOCK;
		return;
		break;
	}
}
#else 
#if defined(linux) && defined (__GLIBC__) && (__GLIBC__ >= 2)
#include <termios.h>
#endif

output_master()
{
	int	n, m;
	char		outbuf[BUFSIZ];
	int		f;
#ifdef SYSV_TERMIOS
	struct termios	tc, tc1;
#else
	struct tchars	tc, tc1;
#endif
	extern int	master;

	if ((n = read (master, outbuf, BUFSIZ)) > 0) {
		m = outbuf[0];
		if (m == TIOCPKT_DATA) {
		
			write_stdout((u_char *)&outbuf[1], n - 1);
		
			fflush (stdout);
			return;
		}
		else if (m & (TIOCPKT_FLUSHREAD | TIOCPKT_FLUSHWRITE)) {
			f = 0;
			if (m & TIOCPKT_FLUSHREAD)
				f |= FREAD;
			if (m & TIOCPKT_FLUSHWRITE)
				f |= FWRITE;
			fflush (stdout);
#ifdef TCFLSH
			ioctl (STDOUT, TCFLSH, &f);
#else
			ioctl (STDOUT, TIOCFLUSH, &f);
#endif
		}
		else if (m & TIOCPKT_NOSTOP) {
#ifdef SYSV_TERMIOS
			ioctl (1, TCGETS, &tc);
			tc.c_cc[VSTOP] = 0;
			tc.c_cc[VSTART] = 0;
			ioctl (STDOUT, TCSETS, &tc);
#else
 
			ioctl (1, TIOCGETC, &tc);
			tc.t_stopc = -1;
			tc.t_startc = -1;
			ioctl (STDOUT, TIOCSETC, &tc);
#endif
		}
		else if (m & TIOCPKT_DOSTOP) {
#ifdef SYSV_TERMIOS
			ioctl (master, TCGETS, &tc1);
			ioctl (STDOUT, TCGETS, &tc);
			tc.c_cc[VSTOP] = tc1.c_cc[VSTOP];
			tc.c_cc[VSTART] = tc1.c_cc[VSTART];
			ioctl (STDOUT, TCSETS, &tc);
#else
			ioctl (master, TIOCGETC, &tc1);
			ioctl (STDOUT, TIOCGETC, &tc);
			tc.t_stopc = tc1.t_stopc;
			tc.t_startc = tc1.t_startc;
			ioctl (STDOUT, TIOCSETC, &tc);
#endif
		}
	}
}
#endif 

set_eucmode ()
{
       eucmode = 1;
}

SJ_write (s, n)
wchar16_t         *s;
int	n;
{
	int	val;
	wchar16_t wb[BUFFLENGTH];
	u_char	buff[BUFFLENGTH * 3];

	wsncpy(wb, s, n);
	wb[n] = 0;
	val = wcstombs((char *) buff, wb, (size_t) n * 3);
	if ((val == 0) && n) {
		int i;
		val = n;
		for (i = 0; i <= n; i++)
		  buff[i] = 0;
	}
	if (val > 0) 
	  val = write(master, buff, val);
	return(val);
}

write_stdout(s, n)
u_char *s;
int n;
{
      static u_char knj1st = 0, knj2nd = 0;

      while (n-- > 0) {
              if (knj1st) {
		      if (knj2nd) {
			      putchar(knj2nd);
			      putchar(knj1st);
			      putchar(*s++);
			      knj2nd = 0;
			      knj1st = 0;
		      } else {
			      putchar(knj1st);
			      putchar(*s++);
			      knj1st = 0;
		      }
	      } else if (knj2nd) {
		      knj1st = *s++;
	      } else if (Isknj2(*s)) {
		      knj2nd = *s++;
	      } else if (Isknj1(*s))
                      knj1st = *s++;
              else
                      putchar(*s++);
      }
}


SJ_read (s, n)
wchar16_t *s;
int	n;
{
	u_char buff[BUFFLENGTH];
	wchar16_t wcbuff[BUFFLENGTH];
	int i, count, remain, c;
	size_t wnum;
	extern int current_locale;

	if (backup) {
		*s = backup;
		backup = 0;
		return (1);
	}
	if ((count = read(STDIN, buff, n)) <= 0)
	        return (count);
	remain = 0;
	for (i = 0; i < count; i++) {
		c = buff[i];
#ifdef __sony_news
		if (is3byte(c)) {
			if (i + 2 >= count) {
				if (i + 2 == count) {
					remain = 1;
					break;
				} else
				        remain = 2;
		        } else
			        i += 2;
		} else if (is2byte(c)) {
			if (i + 1 >= count)
			        remain = 1;
			else
			        i++;
		}
#else
		if (current_locale == LC_CTYPE_EUC) {
			if (iseuc3byte(c)) {
				if (i + 2 >= count) {
					if (i + 2 == count) {
                                        remain = 1;
                                        break;
					} else
                                        remain = 2;
				} else
                                i += 2;
			} else if (iseuc2byte(c)) {
				if (i + 1 >= count)
				  remain = 1;
				else
				  i++;
			}
		} else {
			if (issjis1(c)) {
				if (i + 1 >= count)
				  remain = 1;
				else
				  i++;
			}
		}
#endif

	}
	if (remain) {
		if ((i = read(STDIN, &buff[count], remain)) > 0)
		        count += i;
	}
	buff[count] = 0;
	wnum = mbstowcs((wchar16_t *) wcbuff, (char *) buff, count);
	if ((wnum == 0) && count) {
		wnum = count;
		for ( i = 0; i <= count; i++)
		        wcbuff[i] = (wchar16_t) 0;
	}
	if (wnum > n) {
		backup = wcbuff[--wnum];
	}
        memcpy((void *) s, (void *) wcbuff, wnum * sizeof(wchar16_t));     
	return(wnum);
}

SJ_warnning (s)
wchar16_t	*s;
{
	SJ_print (s);
}

SJ_print (s)
wchar16_t *s;
{
	u_char buff[BUFFLENGTH * 3];
	size_t num;

	num = wcstombs((char *) buff, s, (size_t ) (BUFFLENGTH * 3));
	if ((int) num > 0) 
	  printf("%s", buff);
}

SJ_through (s, n)
wchar16_t *s;
int	n;
{
	SJ_write (s, n);
}

