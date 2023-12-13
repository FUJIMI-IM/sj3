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
 * $SonyRCSfile: depend.c,v $  
 * $SonyRevision: 1.4 $ 
 * $SonyDate: 1997/10/07 07:39:45 $
 *
 * $Id$
 */


#include <sys/types.h>
#include "sj_kcnv.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Dict.h"
#include "sj3err.h"
#include "kanakan.h"


#if defined(__FD_SET) && !defined(FD_SET)
# define FD_SET __FD_SET
# define FD_CLR __FD_CLR
#endif

extern	int	serv_errno;
extern	Global	*work_base;

void	mkidxtbl();

DictFile *dictlink = NULL;
StdyFile *stdylink = NULL;


static long
get4byte(unsigned char* p)
{
	long	i;

	i = *p;
	i = (i << 8) + *++p;
	i = (i << 8) + *++p;
	return ((i << 8) + *++p);
}

static void
Put4byte(unsigned char* p, long n)
{
	p += 3;
	*p-- = n; n >>= 8;
	*p-- = n; n >>= 8;
	*p-- = n; n >>= 8;
	*p = n;
}

#define	put4byte(p, n)	Put4byte((p), (long)(n))


static int
fgetfile(FILE* fp, long pos, int len, void* p)
{
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; return ERROR;
	}
	if (fread(p, len, 1, fp) != 1) {
		serv_errno = SJ3_FileReadError; return ERROR;
	}
	return SJ3_NormalEnd;
}

static int
fputfile(FILE* fp, long pos, int len, unsigned char* p)
{
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; return ERROR;
	}
	if (fwrite(p, len, 1, fp) != 1) {
		serv_errno = SJ3_FileWriteError; return ERROR;
	}
	return SJ3_NormalEnd;
}

static int
getfile(int fd, off_t pos, int len, void* p)
{
	if (lseek(fd, pos, L_SET) == ERROR) {
		serv_errno = SJ3_FileSeekError; return ERROR;
	}
	if (read(fd, p, len) != len) {
		serv_errno = SJ3_FileReadError; return ERROR;
	}
	return SJ3_NormalEnd;
}

static int
putfile(int fd, off_t pos, int len, void* p)
{
	if (lseek(fd, pos, L_SET) == ERROR) {
		serv_errno = SJ3_FileSeekError; return ERROR;
	}
	if (write(fd, p, len) != len) {
		serv_errno = SJ3_FileWriteError; return ERROR;
	}
	return SJ3_NormalEnd;
}


static int
check_passwd(unsigned char* buf, char* passwd)
{
	buf += PasswdPos;
	return (*buf && strncmp(passwd, buf, PasswdLen)) ? FALSE : TRUE;
}

static void
set_passwd(unsigned char* buf, char* passwd)
{
	strncpy(buf + PasswdPos, passwd, PasswdLen);
}


static int
check_dictfile(unsigned char* buf)
{
	return (DictVersion != get4byte(buf + VersionPos)) ? FALSE : TRUE;
}


static DictFile*
search_same_dict(ino_t ino)
{
	DictFile *p;

	for (p = dictlink ; p != NULL ; p = p -> link)
		if (p->dict.dicid == ino) return p;

	return NULL;
}


static int
getofs(DictFile* dp)
{
	idxofs = dp -> ofsptr;
	return 0;
}

static int
getidx(DictFile* dp)
{
	idxbuf = dp -> buffer + dp -> idxstrt;
	return 0;
}

static int
getdic(DictFile* dp, TypeDicSeg seg)
{
	if (seg >= dp->dict.segunit) return -1;
	dicbuf = dp->buffer + dp->segstrt + dp->dict.seglen * seg;
	return 0;
}

static int
putidx(DictFile* dp, TypeDicSeg seg)
{
	return putfile(dp->fd, dp->idxstrt, dp->dict.idxlen, idxbuf);
}

static int
putdic(DictFile* dp, TypeDicSeg seg)
{
	unsigned char	*p;
	long	i, j;

	if (seg >= dp->dict.maxunit) return -1;
	i = dp->segstrt + dp->dict.seglen * (seg + 1);
	if (i > dp->bufsiz) {
		if (!(p = (unsigned char *)malloc(i))) return -1;
		memcpy(dp->buffer, p, dp->bufsiz);

		if (idxbuf - dp->buffer == dp->idxstrt)
			idxbuf = p + dp->idxstrt;
		if ((dicbuf >= dp->buffer) &&
		    ((j = dicbuf - dp->buffer) < dp->bufsiz))
			dicbuf = p + j;

		free((char *)(dp -> buffer));
		dp -> buffer = p;
		dp -> bufsiz = i;
	}

	i = dp->segstrt + dp->dict.seglen * seg;
	if ((p = dp->buffer + i) != dicbuf) memcpy(dicbuf, p, dp->dict.seglen);

	return putfile(dp->fd, i, dp->dict.seglen, p);
}

static int
rszdic(DictFile* dp, TypeDicSeg seg)
{
	long	i;
	unsigned char	*p;

	i = dp->segstrt + dp->dict.seglen * seg;
	if (dp->bufsiz != i) {
		if (!(p = (unsigned char *)malloc(i))) return -1;
		memcpy(dp->buffer, p, ((i > dp->bufsiz) ? dp->bufsiz : i));

		if (idxbuf - dp->buffer == dp->idxstrt)
			idxbuf = p + dp->idxstrt;
		if ((dicbuf >= dp->buffer) &&
		    ((i = dicbuf - dp->buffer) < dp->bufsiz))
			dicbuf = p + i;

		free((char *)(dp -> buffer));
		dp -> buffer = p;
		dp -> bufsiz = i;
		if (ftruncate(dp->fd, i) == -1) return -1;
	}
	put4byte(dp->buffer + DictSegNum, seg);
	return putfile(dp->fd, 0, HeaderLength, dp->buffer);
}


DictFile*
opendict(char* name, char* passwd)
{
	FILE		*fp;
	struct stat	sbuf;
	DictFile	*dfp;
	unsigned char	tmp[HeaderLength];
	int		i;
	unsigned char	*dp;

	
	if (stat(name, &sbuf) == ERROR) {
		if (errno == ENOENT)
			serv_errno = SJ3_FileNotExist;
		else
			serv_errno = SJ3_CannotAccessFile;
		return NULL;
	}

	
	if ((dfp = search_same_dict(sbuf.st_ino)) != NULL) {
		dfp -> refcnt++; return dfp;
	}

	
	if ((fp = fopen(name, "r+")) == NULL) {
		if ((fp = fopen(name, "r")) == NULL) {
			serv_errno = SJ3_CannotOpenFile; return NULL;
		}
		i = FALSE;
	}
	else
		i = TRUE;

	
	if (fgetfile(fp, 0L, sizeof(tmp), tmp) == ERROR) goto error1;

	
	if (!check_dictfile(tmp)) {
		serv_errno = SJ3_IllegalDictFile; goto error1;
	}

	
	if (!check_passwd(tmp, passwd)) {
		serv_errno = SJ3_IncorrectPasswd; goto error1;
	}

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
	if ((dp = (unsigned char *)malloc((long)sbuf.st_size)) == NULL) {
#else
	if ((dp = (unsigned char *)malloc(sbuf.st_size)) == NULL) {
#endif
		serv_errno = SJ3_NotEnoughMemory; goto error1;
	}
	if ((dfp = (DictFile *)malloc(sizeof(*dfp))) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error2;
	}
	memset(dfp, '\0', sizeof(*dfp));

#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__bsdi__) || defined(__DragonFly__)
	if (fgetfile(fp, 0L, (long)sbuf.st_size, dp) == ERROR) goto error3;
#else
	if (fgetfile(fp, 0L, sbuf.st_size, dp) == ERROR) goto error3;
#endif	
	dfp -> dict.dicid   = sbuf.st_ino;
	dfp -> dict.idxlen  = get4byte(dp + DictIdxLen);
	dfp -> dict.seglen  = get4byte(dp + DictSegLen);
	dfp -> dict.segunit = get4byte(dp + DictSegNum);
	dfp -> dict.maxunit = i ? get4byte(dp + DictSegMax) : 0 ;
	dfp -> dict.getofs  = getofs;
	dfp -> dict.getidx  = getidx;
	dfp -> dict.getdic  = getdic;
	dfp -> dict.putidx  = putidx;
	dfp -> dict.putdic  = putdic;
	dfp -> dict.rszdic  = rszdic;
	dfp -> refcnt       = 1;
	dfp -> fp           = fp;
	dfp -> fd           = fileno(fp);
	dfp -> buffer       = dp;
	dfp -> bufsiz       = sbuf.st_size;
	dfp -> idxstrt      = get4byte(dp + DictIdxPos);
	dfp -> segstrt      = get4byte(dp + DictSegPos);

	i = dfp -> dict.segunit;
	if (i < dfp -> dict.maxunit) i = dfp -> dict.maxunit;
	i *= sizeof(TypeIdxOfs);
	if ((dfp -> ofsptr = (TypeIdxOfs *)malloc(i)) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error3;
	}
	mkidxtbl(&(dfp -> dict));

	dfp -> link         = dictlink;
	dictlink = dfp;

	return dfp;

	

error3:	free((char *)dfp);
error2:	free((char *)dp);
error1:	fclose(fp);

	return NULL;
}


int
closedict(DictFile* dfp)
{
	DictFile	*df;

	
	if (--dfp->refcnt > 0) return 0;

	
	if (dictlink == dfp)
		dictlink = dfp -> link;
	else {
		for (df = dictlink ; df ; df = df -> link)
			if (df -> link == dfp) {
				df -> link = dfp -> link;
				break;
			}
	}

	
	if (dfp -> fp) fclose(dfp ->fp);

	
	free((char *)(dfp -> ofsptr));
	free((char *)(dfp -> buffer));
	free((char *)dfp);

	return 0;
}


static	fd_set zero_fd_set = { { 0 } };

void
lock_dict(DictFile* p, int fd)
{
	FD_SET(fd, &(p -> lock));
}

void
unlock_dict(DictFile* p, int fd)
{
	FD_CLR(fd, &(p -> lock));
}

int
is_dict_locked(DictFile* p)
{
	return memcmp(&(p -> lock), &zero_fd_set, sizeof(zero_fd_set));
}


static int
check_stdyfile(unsigned char* buf)
{
	return (StdyVersion != get4byte(buf + VersionPos)) ? FALSE : TRUE;
}


static StdyFile*
search_same_stdy(ino_t ino)
{
	StdyFile *p;

	for (p = stdylink ; p != NULL ; p = p -> link)
		if (p -> inode == ino) return p;

	return NULL;
}


StdyFile*
openstdy(char* name, char* passwd)
{
	FILE		*fp;
	struct stat	sbuf;
	StdyFile	*sfp;
	unsigned char	*hd;
	STDYIN		*sp;
	unsigned short	*cip;
	unsigned char	*clp;
	long		stdycnt, stdypos, stdylen, stdymax;
	long		clidxpos, clidxlen;
	long		clstdypos, clstdylen, clstdystep;
	long		len;

	
	if (stat(name, &sbuf) == ERROR) {
		if (errno == ENOENT)
			serv_errno = SJ3_FileNotExist;
		else
			serv_errno = SJ3_CannotAccessFile;
		return NULL;
	}

	
	if ((sfp = search_same_stdy(sbuf.st_ino)) != NULL) {
		sfp -> refcnt++; return sfp;
	}

	
	if ((hd = (unsigned char *)malloc(HeaderLength+CommentLength)) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; return NULL;
	}

	
	if ((fp = fopen(name, "r+")) == NULL) {
		serv_errno = SJ3_CannotOpenFile; goto error0;
	}

	
	if (fgetfile(fp, 0L, HeaderLength+CommentLength, hd) == ERROR)
		goto error1;

	
	if (!check_stdyfile(hd)) {
		serv_errno = SJ3_IllegalStdyFile; goto error1;
	}

	
	if (!check_passwd(hd, passwd)) {
		serv_errno = SJ3_IncorrectPasswd; goto error1;
	}

	
	stdycnt    = get4byte(hd + StdyNormCnt);
	stdypos    = get4byte(hd + StdyNormPos);
	stdylen    = get4byte(hd + StdyNormLen);
	stdymax    = get4byte(hd + StdyNormNum);
	clidxpos   = get4byte(hd + StdyClIdxPos);
	clidxlen   = get4byte(hd + StdyClIdxLen);
	clstdypos  = get4byte(hd + StdyClSegPos);
	clstdylen  = get4byte(hd + StdyClSegLen);
	clstdystep = get4byte(hd + StdyClIdxStep);
	len = sizeof(STDYIN) * stdymax;

	
	if ((sfp = (StdyFile *)malloc(sizeof(*sfp))) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error1;
	}
	memset(sfp, '\0', sizeof(*sfp));
	if ((sp = (STDYIN *)malloc(len)) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error2;
	}
	if ((cip = (unsigned short *)malloc(clidxlen)) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error3;
	}
	if ((clp = (unsigned char *)malloc(clstdylen)) == NULL) {
		serv_errno = SJ3_NotEnoughMemory; goto error4;
	}

	
	if (fgetfile(fp, clidxpos, clidxlen, cip) == ERROR) goto error5;
	if (fgetfile(fp, clstdypos, clstdylen, clp) == ERROR) goto error5;
	if (fgetfile(fp, stdypos, len, sp) == ERROR) stdycnt = 0;

	
	sfp -> stdy.stdycnt    = stdycnt;
	sfp -> stdy.stdymax    = stdymax;
	sfp -> stdy.stdydic    = sp;
	sfp -> stdy.clstdystep = clstdystep;
	sfp -> stdy.clstdyidx  = cip;
	sfp -> stdy.clstdylen  = clstdylen;
	sfp -> stdy.clstdydic  = clp;
	sfp -> refcnt          = 1;
	sfp -> inode           = sbuf.st_ino;
	sfp -> fp              = fp;
	sfp -> fd              = fileno(fp);
	sfp -> header          = hd;

	sfp -> link            = stdylink;
	stdylink = sfp;

	return sfp;

	

error5:	free((char *)clp);
error4:	free((char *)cip);
error3:	free((char *)sp);
error2:	free((char *)sfp);
error1:	fclose(fp);
error0:	free((char *)hd);

	return NULL;
}


int
closestdy(StdyFile* sfp)
{
	StdyFile	*sf;

	
	if (--sfp->refcnt > 0) return 0;

	
	if (stdylink == sfp)
		stdylink = sfp -> link;
	else {
		for (sf = stdylink ; sf ; sf = sf -> link)
			if (sf -> link == sfp) {
				sf -> link = sfp -> link;
				break;
			}
	}

	
	if (sfp -> fp) fclose(sfp ->fp);

	
	free((char *)sfp -> stdy.stdydic);
	free((char *)sfp -> stdy.clstdyidx);
	free((char *)sfp -> stdy.clstdydic);
	free((char *)sfp -> header);
	free((char *)sfp);

	return 0;
}


int
putstydic(void)
{
	int	fd;
	unsigned char	*hd;
	StdyFile *sf;
	long	len;

	sf = (StdyFile *)stdy_base;
	fd = sf -> fd;
	hd = sf -> header;

	
	put4byte(hd + StdyNormCnt, sf -> stdy.stdycnt);

	
	len = sizeof(STDYIN) * sf -> stdy.stdymax;
	put4byte(hd + StdyNormLen, len);

	
	if (putfile(fd, 0, HeaderLength+CommentLength, hd)) return ERROR;

	
	return putfile(fd, get4byte(hd + StdyNormPos), len, sf -> stdy.stdydic);
}

int
putcldic(void)
{
	int	fd;
	unsigned char	*hd;
	StdyFile *sf;

	sf = (StdyFile *)stdy_base;
	fd = sf -> fd;
	hd = sf -> header;

	if (putfile(fd, get4byte(hd + StdyClIdxPos),
		get4byte(hd + StdyClIdxLen), sf -> stdy.clstdyidx))
		return ERROR;
	return putfile(fd, get4byte(hd + StdyClSegPos),
			get4byte(hd + StdyClSegLen), sf -> stdy.clstdydic);
}


int
makedict(char* path, int idxlen, int seglen, int segnum)
{
	FILE	*fp;
	unsigned char	tmp[HeaderLength + CommentLength];
	long	pos = HeaderLength + CommentLength;
	int	ret = ERROR;
	int	i;

	
	if (idxlen < MinIdxLen || seglen < MinSegLen || segnum < MinSegNum) {
		serv_errno = SJ3_IllegalParameter; return ret;
	}

	if (!(fp = fopen(path, "w"))) {
		serv_errno = SJ3_CannotCreateFile; return ret;
	}
	memset(tmp, '\0', sizeof(tmp));
	put4byte(tmp + VersionPos, DictVersion);

	
	put4byte(tmp + DictIdxPos, pos);
	put4byte(tmp + DictIdxLen, idxlen);
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; goto error;
	}
	for (i = 0 ; i < idxlen ; i++)
		if (putc(0, fp))  {
			serv_errno = SJ3_FileWriteError; goto error;
		}
	pos += idxlen;

	
	put4byte(tmp + DictSegPos, pos);
	put4byte(tmp + DictSegLen, seglen);
	put4byte(tmp + DictSegNum, 1);
	put4byte(tmp + DictSegMax, segnum);
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; goto error;
	}
	if (putc(0, fp)) {
			serv_errno = SJ3_FileWriteError; goto error;
	}
	for (i = 1 ; i < seglen ; i++)
		if (putc(0xff, fp) != 0xff) {
			serv_errno = SJ3_FileWriteError; goto error;
		}

	
	if (fputfile(fp, 0L, sizeof(tmp), tmp) == SJ3_NormalEnd)
		ret = SJ3_NormalEnd;
error:
	fclose(fp);
	if (ret != SJ3_NormalEnd) unlink(path);
	return ret;
}


int
makestdy(char* path, int stynum, int clstep, int cllen)
{
	FILE	*fp;
	unsigned char	tmp[HeaderLength + CommentLength];
	long	pos = HeaderLength + CommentLength;
	int	ret = ERROR;
	int	i, j;

	
	if (stynum == 0) stynum = MinStyNum;
	if (clstep == 0) clstep = MinClStep;
	if (cllen == 0) cllen = MinClLen;

	
	if (stynum < MinStyNum || clstep < MinClStep || cllen < MinClLen) {
		serv_errno = SJ3_IllegalParameter; return ret;
	}
	if (stynum > MaxStyNum || clstep > MaxClStep || cllen > MaxClLen) {
		serv_errno = SJ3_IllegalParameter; return ret;
	}

	if (!(fp = fopen(path, "w"))) {
		serv_errno = SJ3_CannotCreateFile; return ret;
	}
	memset(tmp, '\0', sizeof(tmp));
	put4byte(tmp + VersionPos, StdyVersion);

	
	j = 256 * sizeof(unsigned short);
	put4byte(tmp + StdyClIdxPos, pos);
	put4byte(tmp + StdyClIdxLen, j);
	put4byte(tmp + StdyClIdxStep, clstep);
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; goto error;
	}
	for (i = 0 ; i < j ; i++)
		if (putc(0, fp)) {
			serv_errno = SJ3_FileWriteError; goto error;
		}
	pos += j;

	
	put4byte(tmp + StdyClSegPos, pos);
	put4byte(tmp + StdyClSegLen, cllen);
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; goto error;
	}
	for (i = 0 ; i < cllen ; i++)
		if (putc(0, fp)) {
			serv_errno = SJ3_FileWriteError; goto error;
		}
	pos += cllen;

	
	j = sizeof(STDYIN) * stynum;
	put4byte(tmp + StdyNormPos, pos);
	put4byte(tmp + StdyNormLen, j);
	put4byte(tmp + StdyNormNum, stynum);
	put4byte(tmp + StdyNormCnt, 0);
	if (fseek(fp, pos, 0) == ERROR) {
		serv_errno = SJ3_FileSeekError; goto error;
	}
	for (i = 0 ; i < j ; i++) {
		if (putc(0, fp)) {
			serv_errno = SJ3_FileWriteError; goto error;
		}
	}
	pos += j;

	
	if (fputfile(fp, 0L, sizeof(tmp), tmp) == SJ3_NormalEnd)
		ret = SJ3_NormalEnd;
error:
	fclose(fp);
	if (ret != SJ3_NormalEnd) unlink(path);
	return ret;
}


void
sj_closeall(void)
{
	while (dictlink) closedict(dictlink);
	while (stdylink) closestdy(stdylink);
}


int
set_dictpass(DictFile* dp, char* pass)
{
	set_passwd(dp -> buffer, pass);
	return putfile(dp->fd, 0, HeaderLength+CommentLength, dp->buffer);
}

int
set_stdypass(char* pass)
{
	StdyFile	*sp;

	sp = (StdyFile *)stdy_base;
	set_passwd(sp -> header, pass);
	return putfile(sp->fd, 0, HeaderLength+CommentLength, sp->header);
}



static void
set_comment(unsigned char* buf, char* comment)
{
	strncpy(buf + HeaderLength, comment, CommentLength);
}

int
set_dictcmnt(DictFile* dp, char* cmnt)
{
	set_comment(dp -> buffer, cmnt);
	return putfile(dp->fd, 0, HeaderLength+CommentLength, dp->buffer);
}

int
set_stdycmnt(char* cmnt)
{
	StdyFile	*sp;

	sp = (StdyFile *)stdy_base;
	set_comment(sp -> header, cmnt);
	return putfile(sp->fd, 0, HeaderLength+CommentLength, sp->header);
}


void
get_stdysize(int* stynum, int* clstep, int* cllen)
{
	*stynum = StudyMax;
	*clstep = ClStudyStep;
	*cllen  = ClStudyLen;
}
