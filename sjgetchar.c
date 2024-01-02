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



#include "wchar16.h"
#include "kctype.h"
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>
#include "sj2.h"
#include "select.h"

/* display.c */
void output_master (void);

/* sjgetchar.c */
void write_stdout(unsigned char *, int);
int SJ_read (wchar16_t *, int);
void SJ_print (wchar16_t *);

/* wc16_str.c */
wchar16_t *sj3_wsncpy16(wchar16_t *, wchar16_t *, int);
int sj3_mbstowcs16(wchar16_t *, unsigned char *, int);
int sj3_wcstombs16(unsigned char *, wchar16_t *, int);

#if !defined(FREAD) && defined(O_RDONLY)
#define		FREAD		(O_RDONLY+1)
#define		FWRITE		(O_WRONLY+1)
#endif

#define       Isknj1(s)       ((eucmode) ? (iseuc(s) || (SS2 == (s))) : (issjis1(s)))
#define       Isknj2(s)       ((eucmode) ? (SS3 == (s)) : 0)

static wchar16_t  backup = (wchar16_t) 0;

static int    eucmode = 0;
extern int	master;

wchar16_t SJ_getchar(void)
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

void output_master(void)
{
	int	n, m;
	char		outbuf[BUFSIZ];
	int		f;
	struct termios	tc, tc1;
	extern int	master;

	if ((n = read (master, outbuf, BUFSIZ)) > 0) {
		m = outbuf[0];
		if (m == TIOCPKT_DATA) {
		
			write_stdout((unsigned char *)&outbuf[1], n - 1);
		
			fflush (stdout);
			return;
		}
		else if (m & (TIOCPKT_FLUSHREAD | TIOCPKT_FLUSHWRITE)) {
			switch (m & (TIOCPKT_FLUSHREAD | TIOCPKT_FLUSHWRITE)) {
			case TIOCPKT_FLUSHREAD:
				f = TCIFLUSH;
				break;
			case TIOCPKT_FLUSHWRITE:
				f = TCOFLUSH;
				break;
			default:
				f = TCIOFLUSH;
				break;
			}
			fflush (stdout);
			tcflush (STDOUT, f);
		}
		else if (m & TIOCPKT_NOSTOP) {
			tcgetattr(1, &tc);
			tc.c_cc[VSTOP] = 0;
			tc.c_cc[VSTART] = 0;
			tcsetattr(STDOUT, TCSANOW, &tc);
		}
		else if (m & TIOCPKT_DOSTOP) {
			tcgetattr(master, &tc1);
			tcgetattr(STDOUT, &tc);
			tc.c_cc[VSTOP] = tc1.c_cc[VSTOP];
			tc.c_cc[VSTART] = tc1.c_cc[VSTART];
			tcsetattr(STDOUT, TCSANOW, &tc);
		}
	}
}

void set_eucmode (void)
{
       eucmode = 1;
}

int SJ_write (wchar16_t *s, int n)
{
	int	val;
	wchar16_t wb[BUFFLENGTH];
	unsigned char	buff[BUFFLENGTH * 3];

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

void write_stdout(unsigned char *s, int n)
{
      static unsigned char knj1st = 0, knj2nd = 0;

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


int SJ_read (wchar16_t *s, int n)
{
	unsigned char buff[BUFFLENGTH];
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

void SJ_warnning (wchar16_t *s)
{
	SJ_print (s);
}

void SJ_print (wchar16_t *s)
{
	unsigned char buff[BUFFLENGTH * 3];
	size_t num;

	num = wcstombs((char *) buff, s, (size_t ) (BUFFLENGTH * 3));
	if ((int) num > 0) 
	  printf("%s", buff);
}

void SJ_through (wchar16_t *s, int n)
{
	SJ_write (s, n);
}

