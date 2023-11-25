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
 * $SonyRCSfile: makeseg.c,v $  
 * $SonyRevision: 1.2 $ 
 * $SonyDate: 1994/12/09 11:27:05 $
 */


#include <stdio.h>
#include <sys/types.h>
#include "sj_struct.h"
#include "../../include/Dict.h"

extern	HindoRec *askknj[];
extern	int	askknj_num;
extern	DouonRec *douon_ptr;
extern	FILE	*outfp;
extern	int	douon_num;

static	u_char	index[MainSegmentLength];	
static	int	idxpos = 0;		
static	int	idxnum = 0;		

OffsetRec *real_ofsrec();
u_char	*knjcvt();



static	u_char *set_ofsask(src, dst)
u_char	*src;
u_char	*dst;
{
	int	len;
	u_char	*ptr;
	OffsetRec *ofsrec;

	
	ofsrec = real_ofsrec(src);

	for (len = ofsrec->klen, ptr = ofsrec->kptr ; len > 0 ; ) {
		switch (*ptr & KanjiModeMask) {
		      case ZenHiraAssyuku:
		      case ZenKataAssyuku:
		      case KanjiAssyuku:
			*dst++ = *ptr++;
			len -= 1;
			break;
		      case LeadingHankaku:
#ifndef UseHankakuInDict 
                        fprintf(stderr, "\302\260\300\255\245\263\241\274\245\311\241\246\245\250\245\351\241\274\n");
                        exit(1);
#else
			*dst++ = *ptr++;
			*dst++ = *ptr++;
			len -= 2;
			break;
#endif
		      case OffsetAssyuku:
			fprintf(stderr, "\306\363\275\305\244\313\245\252\245\325\245\273\245\303\245\310\244\254\273\262\276\310\244\265\244\354\244\306\244\244\244\353\n");
			exit(1);
		      case AiAttribute:
			*dst++ = *ptr++;
			*dst++ = *ptr++;
			len -= 2;
			break;
		      case KanjiStrEnd:
			*dst++ = *ptr++;
			len--;
			break;
		      default:
			*dst++ = *ptr++;
			*dst++ = *ptr++;
			len -= 2;
		}
	}

	return dst;
}



make_knjstr(src, len, dst)
u_char	*src;
int	len;
u_char	*dst;
{
	u_char	*keep = dst;

	while (len > 0) {
		switch (*src & KanjiModeMask) {
		      case ZenHiraAssyuku:
		      case ZenKataAssyuku:
		      case KanjiAssyuku:
			*dst++ = *src++;
			len -= 1;
			break;
		      case LeadingHankaku:
#ifndef UseHankakuInDict 
                        fprintf(stderr, "\302\260\300\255\245\263\241\274\245\311\241\246\245\250\245\351\241\274\n");
                        exit(1);
#else
			*dst++ = *src++;
			*dst++ = *src++;
			len -= 2;
			break;
#endif
		      case OffsetAssyuku:
			dst = set_ofsask(src, dst);
			src += 2;
			len -= 2;
			break;
		      case AiAttribute:
			*dst++ = *src++;
			*dst++ = *src++;
			len -= 2;
			break;
		      case KanjiStrEnd:
			*dst++ = *src++;
			len--;
			break;
		      default:
			*dst++ = *src++;
			*dst++ = *src++;
			len -= 2;
                }
        }
	return(dst - keep);
}



static	make_knjask()
{
	int	i, j;
	u_char	*p;
	u_char	buf[128];
	int	len;
	int	tlen = 0;

	
	for (i = 0 ; i < askknj_num ; i++) {

		
		len = make_knjstr(askknj[i] -> kptr, askknj[i] -> klen, buf);

		
		Free(askknj[i] -> kptr);

		
		p = (u_char *)Malloc(len);
		if (!p) {
			fprintf(stderr, "\245\341\245\342\245\352\244\254\302\255\244\352\244\336\244\273\244\363\n");
			exit(1);
		}

		
		memcpy(p, buf, len);
		askknj[i] -> kptr = p;
		askknj[i] -> klen = len;

		
		tlen += len + 1; 
	}

	return tlen;
}


void
makeseg()
{
	u_char	buf[MainSegmentLength];
	DouonRec *drec;
	HinsiRec *hrec;
	KanjiRec *krec;
	int	i, j, k;
	int	slen;
	u_char	*dst;
	u_char	*sdst;
	u_char	*p, *q;

#ifndef	HEHE
#define	datset(i)	{ if (dst >= buf+sizeof(buf)) goto err; *dst++ = i; } 
#else
#define	datset(i)	{ *dst++; i; }
#endif

	
	dst = buf;

	
	i = make_knjask();
	if (i >= 0x100) {
		fprintf(stderr, "\264\301\273\372\260\265\275\314\312\270\273\372\316\363\244\254\304\271\244\271\244\256\244\353\n");
		exit(1);
	}

	
	datset(i); 
	for (i = 0 ; i < askknj_num ; i++) {
		p = askknj[i] -> kptr;
		j = askknj[i] -> klen;
		while (j > 0) {
			k = codesize(*p);
			datset(*p); p++; 
			if (k > 2) {
				datset(*p); p++; 
				datset(*p); p++; 
			} else if (k > 1) {
				datset(*p); p++; 
			}
			j -= k;
		}
		datset(KanjiStrEnd); 
	}

	
	clear_ofsrec();

	
	for (drec = douon_ptr ; drec ; drec = drec->dnext) {

		
		sdst = dst; dst += DouonBlkSizeNumber; 

		
		p = drec -> yptr;
		slen = strlen(p) - drec -> dlen;
		p += slen;
		for (i = drec -> dlen ; i > 0 ; i--) {
			datset(*p); p++; 
		}
		
		for (hrec = drec->hrec ; hrec ; hrec = hrec->hnext) {

			
			datset(hrec -> hinsi); 

			
			for (krec = hrec->krec ; krec ; krec = krec->knext) {

				
				p = q = knjcvt(krec -> kptr, krec -> klen, &i);

				
				set_ofsrec(krec->kptr, krec->klen, dst - buf);

				
				while (i-- > 0) {
					datset(*p); p++; 
				}
				datset(KanjiStrEnd); 
				
				Free(q);
			}

			
			datset(HinsiBlkTerm); 
		}

		
		i = dst - sdst;
		*sdst = ((i >> 8) & 0x0f);
		*(sdst + 1) = (i & 0xff);

		
		if (drec -> dlen > 0x0f) *sdst |= 0x40;
		*(sdst + 2) = (drec -> dlen & 0x0f);

		
		if (slen > 0x0f) *sdst |= 0x80;
		*(sdst + 2) |= ((slen << 4) & 0xf0);
	}
	
	if (dst < buf + sizeof(buf)) {
		datset(DouonBlkTerm);
	}

	
	for (p = douon_ptr -> yptr ; ; ) {
		if (idxpos < MainIndexLength) {
			index[idxpos++] = *p;
		}
		else {
			fprintf(stderr, "\245\244\245\363\245\307\245\303\245\257\245\271\244\254\244\242\244\325\244\354\244\336\244\267\244\277\n");
			exit(1);
		}
		if (!*p++) break;
	}
	if (idxpos < MainIndexLength) {
		index[idxpos] = 0;
	}
	else {
		fprintf(stderr, "\245\244\245\363\245\307\245\303\245\257\245\271\244\254\244\242\244\325\244\354\244\336\244\267\244\277\n");
		exit(1);
	}

	
	i = dst - buf;
	
	while (dst < buf + sizeof(buf)) *dst++ = DouonBlkTerm;

	
	if (Fseek(outfp,
		(long)(HeaderLength + CommentLength + MainIndexLength
			+ idxnum * MainSegmentLength),
		0) < 0) {
		fprintf(stderr, "\275\320\316\317\245\325\245\241\245\244\245\353\244\307\245\267\241\274\245\257\245\250\245\351\241\274\n");
		exit(1);
	}
	if (Fwrite(buf, sizeof(buf), 1, outfp) != 1) {
		fprintf(stderr, "\275\320\316\317\245\325\245\241\245\244\245\353\244\307\245\351\245\244\245\310\245\250\245\351\241\274\n");
		exit(1);
	}
	Fflush(outfp);

	
	printf("\245\273\245\260\245\341\245\363\245\310\310\326\271\346:%d:", idxnum++);
	output_yomi(stdout, douon_ptr -> yptr);
	printf("\t\245\273\245\260\245\341\245\363\245\310\304\271:%d", i);
	printf("\t\306\261\262\273\270\354\277\364:%d\n", douon_num);
	printf("\264\301\273\372\260\265\275\314\312\270\273\372\316\363:");
	for (i = j = 0 ; i < askknj_num ; i++) {
		putchar('\t');
		if (i && j == 0) putchar('\t');
		output_knj(stdout, askknj[i] -> kptr, askknj[i] -> klen);
		printf(":%d", askknj[i] -> hindo + askknj[i] -> exist);
		if (++j >= 4) {
			putchar('\n');
			j = 0;
		}
	}
	if (j) putchar('\n');
	putchar('\n');
	fflush(stdout);

	return;

err:
	fprintf(stderr, "\245\273\245\260\245\341\245\363\245\310\271\275\300\256\303\346\244\313\245\320\245\303\245\325\245\241\244\254\244\242\244\325\244\354\244\277\n");
	fprintf(stderr, "\245\273\245\260\245\341\245\363\245\310\300\350\306\254\244\316\306\311\244\337:");
	output_yomi(stderr, douon_ptr -> yptr);
	fputc('\n', stderr);

	exit(1);
#undef	datset
}

static  void    put4byte(p, n)
u_char  *p;
long    n;
{
        p += 3;
        *p-- = n; n >>= 8;
        *p-- = n; n >>= 8;
        *p-- = n; n >>= 8;
        *p = n;
}
void    makehead(dict_name)
u_char *dict_name;
{
        u_char  header[HeaderLength + CommentLength];
        long    i;
        u_char  *p;

#define IndexPos        (HeaderLength + CommentLength)

        memset(header, '\0', sizeof(header));
        put4byte(header + VersionPos, DictVersion);
        time(&i);
        p = header + CommentPos;
    sprintf((char *)p, "%s : %s", dict_name, ctime(&i));
    while (*p) {
    if (*p == '\n') { *p = 0; break; }
                p++;
}
 
        i = IndexPos;
                        
        put4byte(header + DictIdxPos, i);
        put4byte(header + DictIdxLen, MainIndexLength);
        i += MainIndexLength;

        put4byte(header + DictSegPos, i);
        put4byte(header + DictSegLen, MainSegmentLength);
        put4byte(header + DictSegMax, 0);
        put4byte(header + DictSegNum, idxnum);
        
        put4byte(header + DictAIdxPos, 0);
        put4byte(header + DictAIdxLen, 0);

        put4byte(header + DictASegPos, 0);
        put4byte(header + DictASegLen, 0);

        Fseek(outfp, (long)HeaderPos, 0);
        Fwrite(header, sizeof(header), 1, outfp);
        Fseek(outfp, (long)IndexPos, 0);
        Fwrite(index, MainIndexLength, 1, outfp);

#undef  IndexPos
}
