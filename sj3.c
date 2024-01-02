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
 * $SonyRCSfile: sj3.c,v $  
 * $SonyRevision: 1.7 $ 
 * $SonyDate: 1997/01/23 11:26:30 $
 */


#if defined(__linux__)
#define _GNU_SOURCE
#endif

#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <grp.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#if defined(__linux__)
#include <pty.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#include <libutil.h>
#elif defined(__NetBSD__) || defined(__OpenBSD__)
#include <util.h>
#endif
#include "wchar16.h"
#include "select.h"
#include "common.h"
#include "sj3.h"

/* common.c */
void inputprocess(void);
void InitConversion(void);

/* conv.c */
void EndConversion(void);

/* etc.c */
suseconds_t UsecWeight(int);

/* eucmessage.c */
int init_messages(void);

/* display.c */
void checkterm(void);
void start_guideline(void);
void end_guideline(void);
void RegionSet(int, int);

/* libif.c */
void SJ2_henkan_end(void);
int SJ2_henkan_init(void);

/* screen.c */
void clear_guide_line(void);
void print_guide_line(void);

/* sj3ver.c */
void print_version(void);

/* sjgetchar.c */
void output_master(void);
void set_eucmode(void);

/* sjrc2.c */
void getsjrc(void);
void getsjrk(int);

/* stat_conv.c */
void set_bs(int);

#ifndef _PATH_UTMP
#define _PATH_UTMP "/var/run/utmp"
#endif

#define DEFAULTSHELL	"/bin/sh"

int	Lflag = 0;		
int	shell_flag = 0;		

#if defined(_GNU_SOURCE)
#define progname program_invocation_short_name
#else
#define progname getprogname()
#endif
char	shellname[LONGLENGTH];		
char	home[LONGLENGTH];		
char	user_name[SHORTLENGTH];		
char	term_name[SHORTLENGTH];		
char	shellprog[LONGLENGTH];		
char	tty_name[SHORTLENGTH];		
char	slave_name[SHORTLENGTH];	
char	*shellargs[SHORTLENGTH];	
static char	line[SHORTLENGTH];		
char  *cur_serv;                      

char  my_hname[MAXHOSTNAMELEN];       
char  **serv_list;                    

extern	int	vflag;		
extern	char	**environ;
extern 	int 	status_line;

struct	winsize	Ttysize;
#define ts_lines ws_row
#define ts_cols ws_col
#if !defined(__linux__)
int	Ttyslot;
#endif
int	master;
int	slave;

struct	termios	b;
int	erase_char;
wchar16_t	erase_str[2];	
void	(*sigpipe)(int);

int Pid_shell;                  

int current_locale; 

int main (int argc, char** argv)
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
	done (0);
}

void makecore (__attribute__((unused)) int a)
{
	chdir ("/tmp");
	setgid (getgid ());
	setuid (getuid ());
	abort ();
}


static void (*set_signal_handler (int sig, void (*handler)(int)))(int)
{
	struct sigaction act, oact;
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, sig);
	act.sa_handler = handler;
	act.sa_mask = mask;
	act.sa_flags = SA_RESTART;
	return (sigaction(sig, &act, &oact) < 0) ? NULL : oact.sa_handler;
}


void init (char** argv)
{
	set_signal_handler (SIGTERM, done);
	set_signal_handler (SIGWINCH, SIG_IGN);
	set_signal_handler (SIGQUIT, makecore);
	set_signal_handler (SIGBUS, makecore);
	set_signal_handler (SIGSEGV, makecore);
	set_signal_handler (SIGFPE, makecore);
	sigpipe = set_signal_handler (SIGPIPE, SIG_IGN);

	if (argv [0][0] == '-')
		Lflag ++;

	init_env();
}


void init_env(void)
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


void parsearg (int argc, char** argv)
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


void usage(void)
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


void sjinit (void)
{
	getfixtty ();		
	setshellname ();	
        
        if (cur_serv == NULL)
                cur_serv = serv_list[0] = my_hname;
	SJ2_henkan_init ();
	start_guideline ();
}


void setshellname (void)
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
	else
		strcpy (shellname, "J");
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


void getfixtty (void)
{
	getmaster ();		
	fixtty ();		
	setdev ();		
}


void get_ttymode (void)
{
	tcgetattr (0, &b);
	erase_char = b.c_cc[VERASE];
	set_bs(erase_char);
	erase_str[0] = erase_char;
	erase_str[1] = '\0';

        (void) set_jmode();
}


void getmaster (void)
{
	int n;

	/* not POSIX compatible, but easy to use Linux and *BSD */
	if (openpty (&master, &slave, line, NULL, NULL) < 0) {
		aprintf ("Out of pty's\n\r");
		done3 ();
	}

	n = 1;
	ioctl (master, TIOCPKT, &n);
}


void set_jmode (void)
{
	char *loc;

	loc = setlocale(LC_CTYPE, "");

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
}


void fixtty (void)
{
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
	tcsetattr (0, TCSADRAIN, &sbuf);
}


void setdev (void)
{
	char	*cp;
	struct group	*ttygrp;
	Conversion	*cv;

	cv = current_conversion;

	if ((cp = ttyname (2)) != NULL)
		strcpy (tty_name, cp);
	else
		*tty_name = '\0';

	strcpy (slave_name, line);

	if ((ttygrp = getgrnam ("tty")) != NULL)
		chown(slave_name, getuid(), ttygrp->gr_gid);
	else
		chown(slave_name, getuid(), getgid());
	chmod(slave_name, 0620);

#if !defined(__linux__)
	Ttyslot = ttyslot();
#endif

	if (ioctl(0, TIOCGWINSZ, &Ttysize) == 0)
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

	set_signal_handler (SIGCHLD, exitprocess);
}


static char	shellbuf[128];

void forkshell (void)
{
	Pid_shell = fork ();
	if (Pid_shell < 0) {
		perror ("fork");
		done (0);
	}
	if (Pid_shell == 0) {
		shellprocess ();
	}

	if (vflag > 1) {
		aprintf ("sj3   pid = %d\n\r", getpid ());
		aprintf ("shell pid = %d\n\r", Pid_shell);
	}

	set_signal_handler (SIGHUP, fail);
	set_signal_handler (SIGINT, fail);
	set_signal_handler (SIGTERM, fail);
	set_signal_handler (SIGWINCH, onwinch);
}


void sj3_setenv (char* ename, char* eval, char* buf)
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


void shellprocess (void)
{
#if defined(__linux__)
	struct utmp Utmp;
	
	setsid();
	getslave ();

	memset ((char *) &Utmp, '\0', sizeof (Utmp));
	(void) strncpy(Utmp.ut_user, user_name, sizeof(Utmp.ut_user));
	(void) strncpy(Utmp.ut_line, &slave_name[sizeof("/dev/")-1],
							sizeof(Utmp.ut_line));
	Utmp.ut_type = USER_PROCESS;
	Utmp.ut_pid = (pid_t)getpid();
	strncpy(Utmp.ut_id, &Utmp.ut_line[2], 4);
	Utmp.ut_id[0] = 't';
	Utmp.ut_time = time(0);
	setutent();
	getutid(&Utmp);
	pututline(&Utmp);
	endutent();

#else /* __linux__ */
	struct utmp		Utmp;
	char		*p;
	int		f;
/*
 * Remove warning.
 * Patched by Hidekazu Kuroki(hidekazu@cs.titech.ac.jp)		1996/8/10
 */
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

	if (setsid() < 0) {
		perror("setsid()");
	} else {
		int t = open ("/dev/tty", O_RDWR);
		if (t >= 0) {
			ioctl (t, TIOCNOTTY, (char *)0);
			(void) close(t);
		}
	}

	getslave ();
#endif /* __linux__ */

	(void) close (master);
	dup2 (slave, 0);
	dup2 (slave, 1);
	dup2 (slave, 2);
	(void) close (slave);

#if !defined(__linux__)
	if (Ttyslot > 0 && (f = open (_PATH_UTMP, O_WRONLY)) >= 0) {
		lseek (f, (long) (Ttyslot * sizeof (Utmp)), 0);
		write (f, (char *)&Utmp, sizeof (Utmp));
		close (f);
	}
#endif /* __linux__ */

	setgid (getgid ());
	setuid (getuid ());

	if (shellargs[0] == (char *) NULL) {
		shellargs[0] = shellname;
		shellargs[1] = (char *) 0;
	}

	set_signal_handler (SIGPIPE, sigpipe);
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


void getslave (void)
{
	tcsetattr (slave, TCSAFLUSH, &b);
	(void) ioctl(slave, TIOCSWINSZ, (char *)&Ttysize);
}


void execcmd (char* cmd, char** ap)
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


void fail (__attribute__((unused)) int a)
{
	(void) kill (0, SIGTERM);
	done (0);
}


void done (__attribute__((unused)) int a)
{
	set_signal_handler (SIGWINCH, SIG_IGN);

	clear_guide_line ();
	end_guideline ();
	done2 ();
}


void done2 (void)
{
	chown (slave_name, 0, 0);
	chmod (slave_name, 0666);
	tcsetattr (0, TCSADRAIN, &b);
	if (!status_line) {
		++Ttysize.ts_lines;
		SetRegion();
	}
	clearutmpentry ();
	SJ2_henkan_end ();
	EndConversion ();
	done3 ();
}


void done3 (void)
{
	aprintf ("\r\nexit %s.\r\n", progname);
	exit (0);
}


void exitprocess (__attribute__((unused)) int a)
{
	int		status;

	wait3 (&status, (WNOHANG|WUNTRACED), 0);
	if (WIFSTOPPED (status))
		suspend();
        else
		done (0);
}


void suspend (void)
{
	void		(*old_sigtstp)(int);
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
			fail (0);
	} while (nc > 0);

	tcsetattr (0, TCSADRAIN, &b);

	if (!status_line) {
		++Ttysize.ts_lines;
		SetRegion();
	}

	old_sigtstp = set_signal_handler (SIGTSTP, SIG_DFL);
	kill (0, SIGTSTP);

	set_signal_handler (SIGTSTP, old_sigtstp);
	killpg (Pid_shell, SIGCONT);
	fixtty ();

	if (!status_line) {
		--Ttysize.ts_lines;
		SetRegion();
	}

	print_guide_line ();
}

#if defined(__linux__)
void clearutmpentry (void)
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
#else /* __linux__ */
void clearutmpentry (void)
{
	int			f;
	struct utmp		Utmp;

	Utmp.ut_line[0] = '\0';
	Utmp.ut_name[0] = '\0';
	Utmp.ut_host[0] = '\0';
	Utmp.ut_time = 0;

	if (Ttyslot > 0 && (f = open (_PATH_UTMP, O_WRONLY)) >= 0) {
		lseek (f, (long) (Ttyslot * sizeof (Utmp)), 0);
		write (f, (char *)&Utmp, sizeof (Utmp));
		close (f);
	}
}
#endif /* __linux__ */


void onwinch (__attribute__((unused)) int a)
{
	int ttypgrp;
	Conversion	*cv;

	cv = current_conversion;
	set_signal_handler (SIGWINCH, SIG_IGN);

	if (ioctl (0, TIOCGWINSZ, &Ttysize) == 0)
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

	set_signal_handler (SIGWINCH, onwinch);
}


void SetRegion (void)
{
	set_tty_size ();
	
	if (!status_line)
		RegionSet (1, Ttysize.ts_lines);
}


void set_tty_size (void)
{
	ioctl (master, TIOCSWINSZ, &Ttysize);
}


void setsjserv(char* hname)
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


char *chmyhname(char* hname)
{
        if (hname != NULL) {
                if (strcmp(my_hname, hname) == 0 ||
                    strcmp("localhost", hname) == 0)
                        hname = NULL;
        }
        return(hname);
}
