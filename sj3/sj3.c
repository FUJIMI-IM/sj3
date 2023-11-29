/*-
 * SPDX-License-Identifier: MIT-open-group
 *
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
 */




#include "sj_sysvdef.h"
#include "wchar16.h"
#include <signal.h>
#include <errno.h>
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
#define	USE_OLD_TTY
#if defined(__NetBSD__)
#define LACKOF_SETLOCALE
#endif
#if defined(__FreeBSD__)
#include <sys/ioctl_compat.h>
#endif
#include <curses.h>
#endif

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "select.h"
#include "common.h"
#ifdef SVR4
#include <sys/fcntl.h>
#include <sys/stropts.h>
#include <netdb.h>
#endif
#include <locale.h>
#include "sj3.h"

#ifndef _PATH_UTMP
#define _PATH_UTMP "/etc/utmp"
#endif


#ifdef SVR4
#define killpg(pid, sig) kill(-(pid),(sig))
#define signal sigset
#endif
#ifndef linux
#define	DEFAULTSHELL	"/bin/csh"
#else
#define DEFAULTSHELL	"/bin/sh"
#endif

#if defined(linux) && defined (__GLIBC__) && (__GLIBC__ >= 2)
#include <termios.h>
#endif

int	Lflag = 0;		
int	shell_flag = 0;		



char	progname[LONGLENGTH];		
char	shellname[LONGLENGTH];		
char	home[LONGLENGTH];		
char	user_name[SHORTLENGTH];		
char	term_name[SHORTLENGTH];		
char	shellprog[LONGLENGTH];		
char	tty_name[SHORTLENGTH];		
#ifdef SVR4
char    *slave_name;
#else
char	slave_name[SHORTLENGTH];	
#endif
char	*shellargs[SHORTLENGTH];	
#ifdef SVR4
char	*mptname = "/dev/ptmx";		
char    *ptsname();
#else
static char	line[SHORTLENGTH];		
#endif
char  *cur_serv;                      

char  my_hname[MAXHOSTNAMELEN];       
char  **serv_list;                    



extern	int	vflag;		
extern	char	**environ;
extern 	int 	status_line;




#ifdef SYSV_TERMIOS
struct	winsize	Ttysize;
#define ts_lines ws_row
#define ts_cols ws_col
#else
int	Ttyslot;
struct	ttysize	Ttysize;
#endif
int	master;
int	slave;

#ifdef SYSV_TERMIOS
struct	termios	b;
#else
struct	sgttyb	b;
struct	tchars	tc, tc1;
struct	ltchars	lc, lc1;
int	lb;
int	l;
#endif
#ifdef __sony_news
int	jmode, jmode2;
struct	jtchars	jtc;
#endif
int	erase_char;
wchar16_t	erase_str[2];	
#ifdef SIGTYPE_VOID
void	(*sigpipe)();	
#else
int	(*sigpipe)();	
#endif

int Pid_shell;                  


int current_locale; 

int
main (int argc, char** argv)
{
	init (argv);		
	get_ttymode ();         
	init_messages ();       
	parsearg (argc, argv);	
	InitConversion ();
	checkterm ();		
	getsjrc ();		
	print_version ();	
	getsjrk(erase_char);	
	sjinit ();		
	forkshell ();		
	inputprocess ();	
	done ();		
}

void
makecore (void)
{
	chdir ("/tmp");
	setgid (getgid ());
	setuid (getuid ());
	abort ();
}


void
init (char** argv)
{
	char		*cp;

	(void) signal (SIGTERM, (void (*)())done);
	(void) signal (SIGWINCH, SIG_IGN);
	(void) signal (SIGQUIT, (void (*)())makecore);
	(void) signal (SIGBUS, (void (*)())makecore);
	(void) signal (SIGSEGV, (void (*)())makecore);
	(void) signal (SIGFPE, (void (*)())makecore);
#ifdef SIGTYPE_VOID
	sigpipe = (void(*)()) signal (SIGPIPE, SIG_IGN);
#else
	sigpipe = (int(*)()) signal (SIGPIPE, SIG_IGN);
#endif

	if (argv [0][0] == '-')
		Lflag ++;
	if ((cp = strrchr (argv [0], '/')) == NULL)
		cp = argv [0];
	else
		cp ++;
	strcpy (progname, cp);
	init_env();
}


void
init_env(void)
{
	char *uname, *hp, *tname;
        int  i;
	struct passwd  *pwd, *getpwnam(), *getpwuid();
	char	*getlogin(), *getenv();

	user_name[0] = '\0';
	home[0] = '\0';
	shellprog[0] = '\0';

        gethostname(my_hname, MAXHOSTNAMELEN);
        if ((serv_list = (char **)malloc(MAXSERVER * sizeof(char *))) == NULL){
                fprintf (stderr, "init_env: can't alloc memory\n\r");
                done3();  
        }
        for (i = 0; i < MAXSERVER; i++)
                serv_list[i] = NULL;
        cur_serv = NULL;

	uname = getlogin ();
	hp = getenv("HOME");

	setpwent ();
	if( uname == NULL || *uname == '\0' )
		pwd = getpwuid(getuid());
	else
		pwd = getpwnam(uname);
	if (pwd != NULL) {
		strcpy(user_name, pwd -> pw_name);
		if (hp == NULL)
			hp = pwd->pw_dir;
		endpwent();
	}
	if (hp != NULL)
		strcpy(home, hp);

	if ((tname = getenv("TERM")) == NULL) {
		aprintf ("Can't getenv TERM\n\r");
		perror("getenv");
		exit(1);
	}
	strcpy(term_name, tname);
}


void
parsearg (int argc, char** argv)
{
	int	ap;
	char	c;
	int	sp;
	char	**shella;

	shella = shellargs;
	ap = 1;
	-- argc;
        while (argc > 0) {
		if (*argv[ap] == '-') {
			c = argv[ap][1];
			switch (c) {
			case 'L':
				Lflag ++;
				break;
			case 'E':
				aprintf ("euc mode\n\r");
				set_eucmode ();
				break;
			case 'V':
				vflag = 0;
				break;
			case 'v':
				aprintf ("verbose\n\r");
				vflag ++;
				break;
			case 'f':
				if (argv[ap + 1] == (char *) NULL)
					usage ();
                                strcpy (shellprog, argv[ap+1]);
				shell_flag ++;
				break;
			case 'e':
				if (argv[ap + 1] == (char *) NULL)
					usage ();
                                strcpy (shellprog, argv[++ap]);
                                sp = 0;
				while (argv[ap] != (char *) NULL)
					shella[sp++] = argv[ap++];
				shella[sp] = (char *) NULL;
				break;
#ifdef OKB
			case 'o':
				oasys ++;
				break;
#endif
			case 'H':
				if (argv[ap + 1] == (char *) NULL)
					usage();
 
                                
                                sp = 0;
                                while (argc > 1 && *argv[ap + 1] != '-') {
                                        if (sp >= MAXSERVER)
                                                break;
                                        serv_list[sp++] = argv[ap + 1];
                                        ap++;
                                        argc--;
                                }
                                cur_serv = serv_list[0];
                                

				break;
			case 'h':
			default:
				usage ();
				break;
			}
		}
		argc --;
		ap ++;
	}
        if (cur_serv == NULL || *cur_serv == '\0')
                setsjserv(getenv("SJ3SERV"));
}


void
usage(void)
{
	aprintf ("Usage: %s [-option ...]\n\r", progname);
	aprintf ("\t-option\n\r");
	aprintf ("\t\t-h             \thelp\n\r");
	aprintf ("\t\t-L             \tlogin shell\n\r");
	aprintf ("\t\t-f shellname   \texec shell name\n\r");
	aprintf ("\t\t-e command...  \texec command arguments\n\r");
#ifdef OKB
	aprintf ("\t\t-o             \toyayubi shift keyboard\n\r");
#endif 
        aprintf ("\t\t-H hostname... \tname list of server hosts\n\r");
	if (vflag > 2) {
		aprintf ("\t\t-v          \tverbose mode\n\r");
		aprintf ("\t\t-E             \tEUC mode\n\r");
	}
	exit (1);
}


void
sjinit (void)
{
	getfixtty ();		
	setshellname ();	
        
        if (cur_serv == NULL)
                cur_serv = serv_list[0] = my_hname;
	SJ2_henkan_init ();
	start_guideline ();
}


void
setshellname (void)
{
	char	*s;
	char	*namep;

	if (shellprog[0] == '\0') {
		s = getenv ("SHELL");
		if ((s == NULL) || (*s == '\0') || Lflag)
			strcpy (shellprog, DEFAULTSHELL);
		else
			strcpy (shellprog, s);
	}
	if (Lflag)
		strcpy (shellname, "-");
#ifndef SVR4
	else
		strcpy (shellname, "J");
#endif 
	if ((namep = strrchr (shellprog, '/')) == NULL)
		namep = shellprog;
	else
		namep ++;
	strcat (shellname, namep);
	strcat (shellname, "[");
	strcat (shellname, user_name);
	strcat (shellname, " on ");
	strcat (shellname, progname);
	strcat (shellname, "]");
}


void
getfixtty (void)
{
	getmaster ();		
	fixtty ();		
	setdev ();		
}


void
get_ttymode (void)
{
#ifdef SYSV_TERMIOS
	ioctl (0, TCGETS, (char *)&b);
	erase_char = b.c_cc[VERASE];
	set_bs(erase_char);
	erase_str[0] = erase_char;
	erase_str[1] = '\0';
#else 
	ioctl (0, TIOCGETP, (char *)&b);
	erase_char = b.sg_erase;
	set_bs(erase_char);
	erase_str[0] = erase_char;
	erase_str[1] = '\0';
	ioctl (0, TIOCGETC, (char *)&tc);
	ioctl (0, TIOCGETC, (char *)&tc1);
	ioctl (0, TIOCGETD, (char *)&l);
	ioctl (0, TIOCGLTC, (char *)&lc);
	ioctl (0, TIOCGLTC, (char *)&lc1);
	ioctl (0, TIOCLGET, (char *)&lb);
#endif 
#ifdef __sony_news
	ioctl (0, TIOCKGET, (char *)&jmode);
	jmode2 = set_jmode (jmode);
	ioctl (0, TIOCKGETC, (char *)&jtc);
#else
        (void) set_jmode();
#endif
}


void
getmaster (void)
{
#ifdef SVR4
	struct stat	stb;

	if ((master = open(mptname, O_RDWR)) < 0) { 
		aprintf ("Out of pty's\n\r");
		done3 ();
	}

	ioctl(master, I_PUSH, "pckt");		
#else 
#if defined(__FreeBSD__) || defined(__DragonFly__)
	struct stat	stb;
	int		h, i, j, k1, k2;
#else /* !__FreeBSD__ */
	char		c;
	struct stat	stb;
	int		i, j, k1, k2;
#endif /* __FreeBSD__ */

	strcpy(line, "/dev/ptyXX");

	k1 = strlen("/dev/pty");
	k2 = strlen ("/dev/ptyp");

#if defined(__FreeBSD__) || defined(__DragonFly__)
	for (h = 0; h < 8; h++) {
		line[k1] = "pqrsPQRS"[h];
		line[k2] = '0';
		if (stat (line, &stb) < 0)
			break;
		for (i = 0; i < 32; i++) {
			line[k2] = "0123456789abcdefghijklmnopqrstuv"[i];
#else /* !__FreeBSD__ */
	for (c = 'p'; c <= 'z'; c ++) { 
		line[k1] = c;
		line[k2] = '0';
		if (stat (line, &stb) < 0)
			break;
		for (i = 0; i < 16; i++) {
			line[k2] = "0123456789abcdef"[i];
#endif /* __FreeBSD__ */
			master = open (line, 2);
			if (master >= 0) {
				j = 1;
				ioctl (master, TIOCPKT, &j);
				return;
			}
		}
	}
	aprintf ("Out of pty's\n\r");
	done3 ();
#endif 
}


void
#ifdef __sony_news
set_jmode (int mode)
#else
set_jmode (void)
#endif
{
#ifdef __sony_news
	int	term_mode;

	term_mode = mode & KM_KANJI;
	if (term_mode == KM_JIS) {
		aprintf ("error: tty jis terminal mode\n\r");
		fflush (stdout);
		exit (1);
	}
	if (term_mode == KM_ASCII) {
		aprintf ("error: tty ascii terminal mode\n\r");
		fflush (stdout);
		exit (1);
	}
	else if (term_mode == KM_EUC) {
		set_eucmode ();
		mode &= ~KM_SYSSJIS;
		mode |= KM_SYSEUC;
		(void) setlocale(LC_CTYPE, "ja_JP.EUC");
		current_locale = LC_CTYPE_EUC;
	}
	else if (term_mode == KM_SJIS) {
		mode &= ~KM_SYSEUC;
		mode |= KM_SYSSJIS;
		(void) setlocale(LC_CTYPE, "ja_JP.SJIS");
		current_locale = LC_CTYPE_SHIFTJIS;
        } else {
		aprintf ("error: unknown tty terminal code\n\r");
		fflush (stdout);
		exit (1);
	}
	return (mode);
#else  
	char *loc;

#ifdef LACKOF_SETLOCALE
	loc = getenv("LANG");
#else
	loc = setlocale(LC_CTYPE, "");
#endif
	if (!loc) {
		aprintf("error: bad locale\n");
		fflush(stdout);
		exit(1);
	}
	if (strcmp(loc, "ja_JP.SJIS") == 0) {
		current_locale = LC_CTYPE_SHIFTJIS;
	} else if (strncmp(loc, "ja", 2) == 0 ||
		   strncmp(loc, "Ja", 2) == 0) {
		current_locale = LC_CTYPE_EUC;
		set_eucmode();
	} else {
		aprintf("error: This locale will not use japanese.\n");
		fflush(stdout);
		exit(1);
	}
#endif /* __sony_news */
}


void
fixtty (void)
{
#ifdef SYSV_TERMIOS
	struct termios sbuf;

	sbuf = b;
	sbuf.c_iflag &= ~(INLCR|IGNCR|ICRNL|IUCLC|IXON);
	sbuf.c_oflag &= ~(ONLCR|OPOST);
	sbuf.c_lflag &= ~(ICANON|ECHO);
	sbuf.c_cc[VMIN] = 1;
	sbuf.c_cc[VTIME] = 0;
	sbuf.c_cc[VINTR] = 0;
	sbuf.c_cc[VQUIT] = 0;
	sbuf.c_cc[VSUSP] = 0;
#ifdef VDSUSP
	sbuf.c_cc[VDSUSP] = 0;
#endif /* VDSUSP */
	sbuf.c_cc[VREPRINT] = 0;
	sbuf.c_cc[VDISCARD] = 0;
	sbuf.c_cc[VWERASE] = 0;
	sbuf.c_cc[VLNEXT] = 0;
	(void) ioctl(0, TCSETSW, (char *)&sbuf);
#else 
	struct sgttyb sbuf;

	sbuf = b;
	sbuf.sg_flags &= ~(EVENP|ODDP|ECHO|CRMOD|RAW|TBDELAY);
	sbuf.sg_flags |= CBREAK;
	sbuf.sg_flags |= TANDEM;
	ioctl (0, TIOCSETN, (char *)&sbuf);



	tc1.t_intrc = -1;
	tc1.t_quitc = -1;
	tc1.t_eofc = -1;
	ioctl (0, TIOCSETC, (char *)&tc1);

	lc1.t_suspc = -1;
	lc1.t_dsuspc = -1;
	lc1.t_rprntc = -1;
	lc1.t_flushc = -1;
	lc1.t_werasc = -1;
	lc1.t_lnextc = -1;
	ioctl (0, TIOCSLTC, (char *)&lc1);
#endif 

#ifdef __sony_news
        ioctl (0, TIOCKSET, (char *)&jmode2);
#endif 
}

#ifdef BSD43	
#include <grp.h>
#endif


void
setdev (void)
{
	char	*cp;
#ifdef BSD43	
	struct group	*ttygrp;
#endif
	Conversion	*cv;

	cv = current_conversion;

	if ((cp = ttyname (2)) != NULL)
		strcpy (tty_name, cp);
	else
		*tty_name = '\0';
#ifndef SVR4
	strcpy (slave_name, line);
	slave_name[strlen ("/dev/")] = 't';
#endif 
#ifdef BSD43	
	if ((ttygrp = getgrnam ("tty")) != NULL)
		chown(slave_name, getuid(), ttygrp->gr_gid);
	else
		chown(slave_name, getuid(), getgid());
	chmod(slave_name, 0620);
#else 
	chown(slave_name, getuid(), getgid());
	chmod(slave_name, 0622);
#endif
#ifndef SYSV_TERMIOS
	Ttyslot = nmttyslot (slave_name);
#endif 


#ifdef SYSV_TERMIOS
	if (ioctl(0, TIOCGWINSZ, &Ttysize) == 0)
#else
	if (ioctl(0, TIOCGSIZE, &Ttysize) == 0)
#endif
	{
		if (!status_line)
			--Ttysize.ts_lines;
		if (Ttysize.ts_lines < 1) {
			aprintf ("not enough lines\n\r");
			done3 ();
		}
		if (Ttysize.ts_cols < 1) {
			aprintf ("not enough columns\n\r");
			done3 ();
		}
		cv->line = Ttysize.ts_lines;
		cv->column = Ttysize.ts_cols;
		SetRegion ();
	} else {
	        if (cv->line < 1) cv->line = 24;
	        if (cv->column < 1) cv->column = 80;
		Ttysize.ts_lines = cv->line;
		Ttysize.ts_cols = cv->column;
		if (ioctl(0, TIOCSWINSZ, &Ttysize) < 0) {
		    aprintf("Warnning: Can't get tty size, use rows(%d) and columns(%d).\n\r", 
			    cv->line, cv->column);
		}
		if (!status_line) {
			Ttysize.ts_lines--;
			cv->line--;
		}
		SetRegion ();
	}			
	if (cv->column < MINCOL) {
		if (Hlevel > 1)
			Hlevel = 1;
	}
	else if (HHlevel > 1) {
		Hlevel = 2;
	}

	if (vflag > 1) {
		aprintf ("line %d\n\r", cv->line);
		aprintf ("column %d\n\r", cv->column);
	}

#ifndef SVR4
	(void) signal (SIGCHLD, (void (*)())exitprocess);
#endif 
}


static char	shellbuf[128];

void
forkshell (void)
{
#ifdef SVR4
  struct sigaction action; 
  sigset_t sset;
#endif
	Pid_shell = fork ();
	if (Pid_shell < 0) {
		perror ("fork");
		done ();
	}
	if (Pid_shell == 0) {
		shellprocess ();
	}

	if (vflag > 1) {
		aprintf ("sj3   pid = %d\n\r", getpid ());
		aprintf ("shell pid = %d\n\r", Pid_shell);
	}

	(void) signal (SIGHUP, (void (*)())fail);
	(void) signal (SIGINT, (void (*)())fail);
	(void) signal (SIGTERM, (void (*)())fail);
	(void) signal (SIGWINCH, (void (*)())onwinch);
#ifdef SVR4
        (void) sigemptyset(&sset);
        (void) sigaddset(&sset, SIGCHLD);
        action.sa_handler = (void (*)()) exitprocess;
        action.sa_mask =  sset;
        action.sa_flags = SA_RESTART;
        (void) sigaction(SIGCHLD, &action, NULL);
#endif
}


void
sj3_setenv (char* ename, char* eval, char* buf)
{
	char *cp, *dp;
	char **ep = environ;

	

	while ((dp = *ep++) != NULL) {
		for (cp = ename; *cp == *dp && *cp; cp++, dp++)
			continue;
		if (*cp == 0 && (*dp == '=' || *dp == 0)) {
			strcat(buf, eval);
			*--ep = buf;
			return;
		}
	}
}


void
shellprocess (void)
{
	int	t, f;
#ifdef SYSV_TERMIOS
#ifdef SYSV_UTMP
	struct utmp Utmp;
#else
	struct utmpx Utmp;
#endif

	
	setsid();
	getslave ();

	memset ((char *) &Utmp, '\0', sizeof (Utmp));
	(void) strncpy(Utmp.ut_user, user_name, sizeof(Utmp.ut_user));
	(void) strncpy(Utmp.ut_line, &slave_name[sizeof("/dev/")-1],
							sizeof(Utmp.ut_line));
	Utmp.ut_type = USER_PROCESS;
#ifdef SYSV_UTMP
	Utmp.ut_pid = (pid_t)getpid();
	strncpy(Utmp.ut_id, &Utmp.ut_line[2], 4);
	Utmp.ut_id[0] = 't';
	Utmp.ut_time = time(0);
	setutent();
	getutid(&Utmp);
	pututline(&Utmp);
	endutent();
#else
	Utmp.ut_pid = (o_pid_t)getpid();
	Utmp.ut_id[0] = 's';
	Utmp.ut_id[1] = 'j';
	Utmp.ut_id[2] = SC_WILDC;
	Utmp.ut_id[3] = SC_WILDC;
	Utmp.ut_exit.e_termination = 0;
	Utmp.ut_exit.e_exit = 0;
	(void) time (&Utmp.ut_tv.tv_sec);
	if (makeutx(&Utmp) == NULL)
		aprintf ("%s:\tmakeutx failed\r\n", progname);
#endif /* SYSV_UTMP */
#else /* SYSV_TERMIOS */
	struct utmp		Utmp;
	char		*p;
#define SCPYN(a, b) strncpy(a, b, sizeof(a))

	if ((p = ttyname (0)) == NULL)
		SCPYN(Utmp.ut_host, progname);
	else {
		SCPYN(Utmp.ut_host, progname);
		strcat (Utmp.ut_host, " on ");
		p = strrchr (p, '/') + 1;
		strcat (Utmp.ut_host, p);
	}
	SCPYN(Utmp.ut_name, user_name);
	SCPYN(Utmp.ut_line, strrchr (slave_name, '/') + 1);
	time (&Utmp.ut_time);

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
	if (setsid() < 0)
		perror("setsid()");
#else
	t = open ("/dev/tty", 2);
	if (t >= 0) {
		ioctl (t, TIOCNOTTY, (char *)0);
		(void) close(t);
	}
#endif  

	getslave ();
#endif /* SYSV_TERMIOS */
	(void) close (master);
	dup2 (slave, 0);
	dup2 (slave, 1);
	dup2 (slave, 2);
	(void) close (slave);

#ifndef SYSV_TERMIOS
	if (Ttyslot > 0 && (f = open (_PATH_UTMP, 1)) >= 0) {
		lseek (f, (long) (Ttyslot * sizeof (Utmp)), 0);
		write (f, (char *)&Utmp, sizeof (Utmp));
		close (f);
	}
#endif /* SYSV_TERMIOS */

	setgid (getgid ());
	setuid (getuid ());

	if (shellargs[0] == (char *) NULL) {
		shellargs[0] = shellname;
		shellargs[1] = (char *) 0;
	}

	(void) signal (SIGPIPE, (void (*)())sigpipe);
	if ( *shellprog == '/' ) {
		if (shell_flag) {
			strcpy (shellbuf, "SHELL=");
			sj3_setenv("SHELL", shellprog, shellbuf);
		}
		(void) execve (shellprog, shellargs, environ);
	}
	else
		execcmd (shellprog, shellargs);

	perror (shellprog);
	exit (1);
}


void
getslave (void)
{
#ifdef SVR4
	grantpt(master);		 
	unlockpt(master);		 
	slave_name = ptsname(master);	 
	slave = open(slave_name, O_RDWR); 
	if (slave < 0) {		 
		perror(slave_name);
		done2();
	}
	ioctl(slave, I_PUSH, "ptem");	  
	ioctl(slave, I_PUSH, "ldterm");	  
	ioctl(slave, I_PUSH, "ttcompat"); 

	(void) ioctl(slave, TCSETSF, (char *)&b);
	(void) ioctl(slave, TIOCSWINSZ, (char *)&Ttysize);
#ifdef __sony_news
	(void) ioctl(slave, TIOCKSET, (char *)&jmode);
	(void) ioctl(slave, TIOCKSETC, (char *)&jtc);
#endif
#else
	line[strlen ("/dev/")] = 't';
	slave = open (line, 2);
	if (slave < 0) {
		perror (line);
		done2 ();
	}
#ifndef SYSV_TERMIOS
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
	ioctl(slave, TIOCSCTTY, NULL);
#endif
	ioctl (slave, TIOCSETP, (char *)&b);
	ioctl (slave, TIOCSETC, (char *)&tc);
	ioctl (slave, TIOCSLTC, (char *)&lc);
	ioctl (slave, TIOCLSET, (char *)&lb);
	ioctl (slave, TIOCSETD, (char *)&l);
#ifdef __sony_news
	ioctl (slave, TIOCKSET, (char *)&jmode);
	ioctl (slave, TIOCKSETC, (char *)&jtc);
#endif
	ioctl (slave, TIOCSSIZE, &Ttysize);
#else /* SYSV_TERMIOS */
	(void) ioctl(slave, TCSETSF, (char *)&b);
	(void) ioctl(slave, TIOCSWINSZ, (char *)&Ttysize);
#endif /* SYSV_TERMIOS */
#endif /* SVR4 */
}


void
execcmd (char* cmd, char** ap)
{
	int	c;
	char	*getenv ();
	char	*path, *cp, fullcmd[BUFSIZ];

	path = getenv ("PATH");
	cp = fullcmd;

	while ((c = *path++) != 0) {
		if (c == ':') {
			*cp++ = '/';
			*cp = '\0';
			cp = fullcmd;
			(void) strcat (cp, cmd);
			if (shell_flag) {
				strcpy (shellbuf, "SHELL=");
				sj3_setenv ("SHELL", cp, shellbuf);
			}
			(void) execve (cp, ap, environ);
			cp = fullcmd;
		} else
			*cp++ = c;
	}

	*cp++ = '/';
	*cp = '\0';
	cp = fullcmd;
	(void) strcat (cp, cmd);
	if (shell_flag) {
		strcpy (shellbuf, "SHELL=");
		sj3_setenv ("SHELL", cp, shellbuf);
	}
	(void) execve (cp, ap, environ);
}


void
fail (void)
{
	(void) kill (0, SIGTERM);
	done ();
}


void
done (void)
{
	(void) signal (SIGWINCH, SIG_IGN);

	clear_guide_line ();
	end_guideline ();
	done2 ();
}


void
done2 (void)
{
	chown (slave_name, 0, 0);
	chmod (slave_name, 0666);
#ifdef SYSV_TERMIOS
	ioctl (0, TCSETSW, (char *)&b);		
#else /* SYSV_TERMIOS */
	ioctl (0, TIOCSETP, (char *)&b);	
	ioctl (0, TIOCSETC, (char *)&tc);	
	ioctl (0, TIOCSLTC, (char *)&lc);	
#endif /* SYSV_TERMIOS */
#ifdef __sony_news
	ioctl (0, TIOCKSET, (char *)&jmode);	
#endif 
	if (!status_line) {
		++Ttysize.ts_lines;
		SetRegion();
	}
	clearutmpentry ();
	SJ2_henkan_end ();
	EndConversion ();
	done3 ();
}


void
done3 (void)
{
	aprintf ("\r\nexit %s.\r\n", progname);
	exit (0);
}


void
exitprocess (void)
{
#ifdef SVR4
        siginfo_t       info;

        (void) waitid (P_ALL, 0, &info, 
		      WNOHANG | WUNTRACED | WEXITED | WTRAPPED | WCONTINUED);
        if ((info.si_code == CLD_CONTINUED) ||(info.si_code == CLD_TRAPPED))
           return;
        if (info.si_code == CLD_STOPPED)
#else
	int		pid, status;

	pid = wait3 (&status, (WNOHANG|WUNTRACED), 0);
	if (WIFSTOPPED (status))
#endif
		suspend();
        else
		done ();
}


void
suspend (void)
{
#ifdef SIGTYPE_VOID
	void		(*old_sigtstp) ();
#else
	int		(*old_sigtstp) ();
#endif
	SELECT_FD	ifds;
	int		nc;
	struct timeval	t;

	clear_guide_line ();

	do {
cont:
		t.tv_sec = 0;
		t.tv_usec = UsecWeight (1);	
		FD_ZERO (&ifds);
		FD_SET (master, &ifds);
		if (select (master + 1, &ifds, 0, 0, &t) > 0) {
			if (FD_ISSET (master, &ifds))
				output_master ();
		}
		else if (errno == EINTR) {
			errno = 0;
			goto cont;
		}
		if (ioctl (master, TIOCOUTQ, &nc) < 0)
#ifdef SVR4
                        break;
#else
			fail ();
#endif
	} while (nc > 0);

#ifdef SYSV_TERMIOS
	ioctl (0, TCSETSW, (char *)&b);		
#else /* SYSV_TERMIOS */
	ioctl (0, TIOCSETP, (char *)&b);	
	ioctl (0, TIOCSETC, (char *)&tc);	
	ioctl (0, TIOCSLTC, (char *)&lc);	
#endif /* SYSV_TERMIOS */
#ifdef __sony_news
	ioctl (0, TIOCKSET, (char *)&jmode);	
#endif 

	if (!status_line) {
		++Ttysize.ts_lines;
		SetRegion();
	}

#ifdef SIGTYPE_VOID
	old_sigtstp = (void(*)()) signal (SIGTSTP, SIG_DFL);
#else
	old_sigtstp = (int(*)()) signal (SIGTSTP, SIG_DFL);
#endif

	kill (0, SIGTSTP);

	

	(void) signal (SIGTSTP, (void (*)())old_sigtstp);
	killpg (Pid_shell, SIGCONT);
	fixtty ();

	if (!status_line) {
		--Ttysize.ts_lines;
		SetRegion();
	}

	print_guide_line ();
}

#ifdef SVR4
void
clearutmpentry(void)
{
	struct utmpx		*up;
#ifdef SVR4
  struct sigaction action;
  sigset_t sset;

  sigemptyset(&sset);
  sigaddset(&sset, SIGCHLD);
  action.sa_handler = SIG_IGN;
  action.sa_mask =  sset;
  action.sa_flags = SA_RESTART;
  (void) sigaction(SIGCHLD, &action, NULL);
#else
	signal(SIGCHLD, SIG_IGN); 
#endif
	setutxent();
	while ( (up = getutxent()) ) {
		if (up->ut_pid != (o_pid_t)Pid_shell)
			continue;
		up->ut_type = DEAD_PROCESS;
		up->ut_exit.e_termination = 0;
		up->ut_exit.e_exit = 0;
		(void) time (&up->ut_tv.tv_sec);
		if (modutx(up) == NULL)
			aprintf("%s:\tmodutx failed\r\n", progname);
		break;
	}
	endutxent();
#ifdef SVR4
  action.sa_handler = (void (*)()) exitprocess;
  action.sa_mask =  sset;
  action.sa_flags = SA_RESTART;
  (void) sigaction(SIGCHLD, &action, NULL);
#else
	signal (SIGCHLD, (void (*)())exitprocess);
#endif
}
#else /* SVR4 */
#ifdef SYSV_UTMP
void
clearutmpentry (void)
{
	struct utmp             Utmp;

	memset ((char *) &Utmp, '\0', sizeof (Utmp));
	(void) strncpy(Utmp.ut_user, user_name, sizeof(Utmp.ut_user));
	(void) strncpy(Utmp.ut_line, &slave_name[sizeof("/dev/")-1],
							sizeof(Utmp.ut_line));
	Utmp.ut_type = DEAD_PROCESS;
	Utmp.ut_pid = (pid_t)getpid();
	strncpy(Utmp.ut_id, &Utmp.ut_line[2], 4);
	Utmp.ut_id[0] = 't';
	Utmp.ut_time = time(0);
	setutent();
	getutid(&Utmp);
	pututline(&Utmp);
	endutent();
}
#else /* SYSV_UTMP */
void
clearutmpentry (void)
{
	int			f;
	struct utmp		Utmp;

	Utmp.ut_line[0] = '\0';
	Utmp.ut_name[0] = '\0';
	Utmp.ut_host[0] = '\0';
	Utmp.ut_time = 0;

	if (Ttyslot > 0 && (f = open (_PATH_UTMP, 1)) >= 0) {
		lseek (f, (long) (Ttyslot * sizeof (Utmp)), 0);
		write (f, (char *)&Utmp, sizeof (Utmp));
		close (f);
	}
}
#endif /* SYSV_UTMP */
#endif /* SVR4 */


void
onwinch (void)
{
	int ttypgrp;
	Conversion	*cv;

	cv = current_conversion;
	(void) signal (SIGWINCH, SIG_IGN);

#ifdef SYSV_TERMIOS
	if (ioctl (0, TIOCGWINSZ, &Ttysize) == 0)
#else /* SYSV_TERMIOS */
	if (ioctl (0, TIOCGSIZE, &Ttysize) == 0)
#endif /* SYSV_TERMIOS */
	{
		if (!status_line) {
			clear_guide_line ();
			--Ttysize.ts_lines;
		}

		cv->line = Ttysize.ts_lines;
		cv->column = Ttysize.ts_cols;

		if (cv->column < MINCOL) {
			if (Hlevel > 1)
				Hlevel = 1;
		}
		else if (HHlevel > 1) {
			Hlevel = 2;
		}

		SetRegion();
		if (!status_line) {
			print_guide_line ();
		}

		

		killpg (Pid_shell, SIGWINCH);

		if (ioctl (master, TIOCGPGRP, &ttypgrp) == 0)
			killpg (ttypgrp, SIGWINCH);
	}

	(void) signal (SIGWINCH, (void (*)())onwinch);
}


void
SetRegion (void)
{
	set_tty_size ();
	
	if (!status_line)
		RegionSet (1, Ttysize.ts_lines);
}


void
set_tty_size (void)
{
	
#ifdef SYSV_TERMIOS
	ioctl (master, TIOCSWINSZ, &Ttysize);
#else /* SYSV_TERMIOS */
	ioctl (master, TIOCSSIZE, &Ttysize);
#endif /* SYSV_TERMIOS */
}


void
setsjserv(char* hname)
{
        char *p;
        int i;
        char *sbuf;
 
        if (hname == NULL || *hname == '\0')    
 		return;
        if ((sbuf = (char *)malloc(strlen(hname))) == NULL) {
                fprintf(stderr, "setsjserv: can't alloc memory\n\r");
                done3();
        }
        strcpy(sbuf, hname);
        p = sbuf;
        for (i = 0; i < MAXSERVER; i++) {
                serv_list[i] = p;
                while(*p != '\0' && *p != ' ' && *p != '\t')
                        p++;
                if (*p == '\0')
                        break;
                *p++ = '\0';
        }
        cur_serv = serv_list[0];
}


char *
chmyhname(char* hname)
{
        if (hname != NULL) {
                if (strcmp(my_hname, hname) == 0 ||
                    strcmp("localhost", hname) == 0)
                        hname = NULL;
        }
        return(hname);
}
