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
 * $SonyRCSfile: sj_rename.h,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:01:14 $
 */




#ifndef	_SJ_RENAME

#define	_SJ_RENAME



#define	adddic		Jadddic
#define	checksub	Jchecksub
#define	checkdict	Jcheckdict
#define	cal_nextym	Jcal_nextym
#define	cmpknj		Jcmpknj
#define	usr_freelen	Jusr_freelen
#define	sprt_seg	Jsprt_seg
#define	apnd_uidx	Japnd_uidx



#define	set_size	Jset_size
#define	isvyomi		Jisvyomi
#define	isvknj		Jisvknj
#define	isgrm		Jisgrm
#define	addel_arg	Jaddel_arg
#define	yomi2kata	Jyomi2kata
#define	top_strcmp	Jtop_strcmp
#define	last_strcmp	Jlast_strcmp
#define	cvtknj		Jcvtknj
#define	srchkana	Jsrchkana
#define	srchgram	Jsrchgram
#define	srchkanji	Jsrchkanji
#define	count_uidx	Jcount_uidx
#define	chg_uidx	Jchg_uidx



#define	cl2knj		Jcl2knj
#define	nextcl		Jnextcl
#define	nextrecblk	Jnextrecblk
#define	prevcl		Jprevcl
#define	prevrecblk	Jprevrecblk
#define	initkbuf	Jinitkbuf
#define	selectnum	Jselectnum
#define	kouhonum	Jkouhonum



#define	clstudy		Jclstudy
#define	delcldata	Jdelcldata
#define	delclsub	Jdelclsub
#define	delclold	Jdelclold
#define	srchclpos	Jsrchclpos
#define	mkclidx		Jmkclidx



#define	cvtclknj	Jcvtclknj



#define	cvtdict		Jcvtdict
#define	cvtminasi	Jcvtminasi
#define	cvtwakachi	Jcvtwakachi



#define	cvtphknj	Jcvtphknj
#define	cvtkouho	Jcvtkouho
#define	setstyrec	Jsetstyrec



#define	deldic		Jdeldic
#define	del_douon	Jdel_douon
#define	del_segment	Jdel_segment
#define	del_uidx	Jdel_uidx
#define	del_stdy	Jdel_stdy



#define	get_askknj	Jget_askknj
#define	seldict		Jseldict
#define	sel_udict	Jsel_udict
#define	get_idxofs	Jget_idxofs
#define	get_idxptr	Jget_idxptr



#define	fzkstrcmp	Jfzkstrcmp
#define	setclrec	Jsetclrec
#define	fzkcnct		Jfzkcnct
#define	srchfzk		Jsrchfzk



#define	getkan_none	Jgetkan_none
#define	getkan_hira	Jgetkan_hira
#define	getkan_kata	Jgetkan_kata
#define	getkan_knj	Jgetkan_knj
#define	getkan_ofs	Jgetkan_ofs
#define	getkan_norm	Jgetkan_norm
#define	code2kanji	Jcode2kanji
#define	getkanji	Jgetkanji



#define	getrank		Jgetrank
#define	setstynum	Jsetstynum
#define	setnspr		Jsetnspr
#define	regetrank	Jregetrank



#define	work_base	Jwork_base
#define	stdy_base	Jstdy_base


#define	maxjptr		(work_base -> Jmaxjptr)
#define	maxclptr	(work_base -> Jmaxclptr)
#define	jrt1st		(work_base -> Jjrt1st)
#define	clt1st		(work_base -> Jclt1st)
#define	jrt2nd		(work_base -> Jjrt2nd)
#define	clt2nd		(work_base -> Jclt2nd)
#define	selcl		(work_base -> Jselcl)
#define	selectid	(work_base -> Jselectid)
#define	trank		(work_base -> Jtrank)
#define	nrank		(work_base -> Jnrank)
#define	buninf		(work_base -> Jbuninf)
#define	buncnt		(work_base -> Jbuncnt)
#define	knjnew		(work_base -> Jknjnew)
#define	knjidx		(work_base -> Jknjidx)
#define	knjmax		(work_base -> Jknjmax)
#define	score		(work_base -> Jscore)
#define	pathnum		(work_base -> Jpathnum)
#define	usedpath	(work_base -> Jusedpath)
#define	inputyomi	(work_base -> Jinputyomi)
#define	cnvstart	(work_base -> Jcnvstart)
#define	cnvlen		(work_base -> Jcnvlen)
#define	ystart		(work_base -> Jystart)
#define	hyomi		(work_base -> Jhyomi)
#define	orgyomi		(work_base -> Jorgyomi)
#define	kanjibuf	(work_base -> Jkanjibuf)
#define	kanjipos	(work_base -> Jkanjipos)
#define	kanjitmp	(work_base -> Jkanjitmp)
#define	kanjilen	(work_base -> Jkanjilen)
#define	dicinl		(work_base -> Jdicinl)
#define	dicsaml		(work_base -> Jdicsaml)
#define	prevseg		(work_base -> Jprevseg)
#define	fzkcount	(work_base -> Jfzkcount)
#define	fzk_ka_flg	(work_base -> Jfzk_ka_flg)
#define	fzktbl		(work_base -> Jfzktbl)
#define	headcode	(work_base -> Jheadcode)
#define	headlen		(work_base -> Jheadlen)
#define	gobilen		(work_base -> Jgobilen)
#define	prevclgrm	(work_base -> Jprevclgrm)
#define	prevclrow	(work_base -> Jprevclrow)
#define	nextcllen	(work_base -> Jnextcllen)
#define	khcount		(work_base -> Jkhcount)
#define	nkhcount	(work_base -> Jnkhcount)
#define	kouhotbl	(work_base -> Jkouhotbl)
#define	stdytop		(work_base -> Jstdytop)
#define	curdict		(work_base -> Jcurdict)
#define	dictlist	(work_base -> Jdictlist)
#define	dicbuf		(work_base -> Jdicbuf)
#define	idxbuf		(work_base -> Jidxbuf)
#define	idxofs		(work_base -> Jidxofs)
#define	askknj		(work_base -> Jaskknj)
#define	askknj_k	(work_base -> Jaskknj_k)

#define	grp_ptr		(work_base -> Jgrp_ptr)
#define	grp_seg		(work_base -> Jgrp_seg)
#define	grp_grp		(work_base -> Jgrp_grp)
#define	suuji_wbuf	(work_base -> Jsuuji_wbuf)
#define	suuji_ubuf	(work_base -> Jsuuji_ubuf)
#define	suuji_class	(work_base -> Jsuuji_class)
#define	suuji_comma	(work_base -> Jsuuji_comma)
#define	suuji_keta	(work_base -> Jsuuji_keta)
#define	suuji_yptr	(work_base -> Jsuuji_yptr)
#define	suuji_len	(work_base -> Jsuuji_len)
#define	suuji_exit	(work_base -> Jsuuji_exit)
#define	suuji_wkeep	(work_base -> Jsuuji_wkeep)
#define suuji_ukeep	(work_base -> Jsuuji_ukeep)
#define	peepyomi	(work_base -> Jpeepyomi)
#define	peepknj		(work_base -> Jpeepknj)
#define	peepgrm		(work_base -> Jpeepgrm)
#define	peepdptr	(work_base -> Jpeepdptr)
#define	peephptr	(work_base -> Jpeephptr)
#define	peepkptr	(work_base -> Jpeepkptr)
#define	peepidx		(work_base -> Jpeepidx)



#define	seg_count	Jseg_count
#define	mkidxtbl	Jmkidxtbl
#define	initwork	Jinitwork



#define	free_jlst	Jfree_jlst
#define	free_clst	Jfree_clst
#define	free_clall	Jfree_clall
#define	free_jall	Jfree_jall
#define	freework	Jfreework
#define	free_kjall	Jfree_kjall



#define	set2nd		Jset2nd
#define	mk2claus	Jmk2claus



#define	mkbunsetu	Jmkbunsetu
#define	argclrec	Jargclrec



#define	mkjiritu	Jmkjiritu
#define	dic_mu		Jdic_mu
#define	argjrec		Jargjrec
#define	dic_cl		Jdic_cl



#define	mkkouho		Jmkkouho
#define	getkhtbl	Jgetkhtbl
#define	cl_kanji	Jcl_kanji
#define	sameknj		Jsameknj
#define	skipatr		Jskipatr
#define	diffknj		Jdiffknj
#define	chrck_numtbl	Jchrck_numtbl
#define	sel_sjmode	Jsel_sjmode
#define	cl_numcmn	Jcl_numcmn



#define	word2char	Jword2char
#define	words2num	Jwords2num
#define	num_type00	Jnum_type00
#define	num_type01	Jnum_type01
#define	num_type02	Jnum_type02
#define	num_type03	Jnum_type03
#define	num_type04	Jnum_type04
#define	num_type05	Jnum_type05
#define	num_type06	Jnum_type06
#define	num_type07	Jnum_type07
#define	kan_num		Jkan_num
#define	num_type08	Jnum_type08
#define	num_type09	Jnum_type09
#define	num_type10	Jnum_type10
#define	num_type11	Jnum_type11
#define	num_type12	Jnum_type12
#define	num_kurai	Jnum_kurai
#define	num_type13	Jnum_type13
#define	num_type14	Jnum_type14



#define	getusr		Jgetusr
#define	nextusr		Jnextusr
#define	prevusr		Jprevusr
#define	set_kanji	Jset_kanji
#define	set_buf		Jset_buf
#define	prev_kanji	Jprev_kanji
#define	prev_hinsi	Jprev_hinsi
#define	prev_douon	Jprev_douon
#define	next_kanji	Jnext_kanji
#define	next_hinsi	Jnext_hinsi
#define	next_douon	Jnext_douon
#define	add_yomi	Jadd_yomi
#define	cd2sjh_chr	Jcd2sjh_chr



#define	ph2knj	Jph2knj



#define	ph_setsty	Jph_setsty
#define	nextkptr	Jnextkptr
#define	maxpset		Jmaxpset
#define	mkknjmax	Jmkknjmax
#define	ph_khtbl	Jph_khtbl



#define	isfukusi	Jisfukusi
#define	istaigen	Jistaigen
#define	taicnt		Jtaicnt
#define	sttcnt		Jsttcnt
#define	priority	Jpriority
#define	pritiny		Jpritiny



#define	selclrec	Jselclrec



#define	initknjall	Jinitknjall
#define	knjtouroku	Jknjtouroku
#define	knjitti		Jknjitti
#define	hikakusub	Jhikakusub
#define	knjhikaku	Jknjhikaku
#define	setgroupai	Jsetgroupai
#define	select_dsub	Jselect_dsub
#define	select_tsub	Jselect_tsub
#define	select_ysub	Jselect_ysub
#define	selectknj	Jselectknj
#define	selknj		Jselknj



#define	cnjstrcmp	Jcnjstrcmp
#define	cnvrow		Jcnvrow
#define	setconj		Jsetconj



#define	setj_atrb	Jsetj_atrb
#define	setj_ofs	Jsetj_ofs
#define	setj_knj	Jsetj_knj
#define	setj_norm1	Jsetj_norm1
#define	setj_norm2	Jsetj_norm2
#define	chkhead		Jchkhead
#define	setjrec		Jsetjrec
#define	setnumrec	Jsetnumrec
#define	setcrec		Jsetcrec



#define	setkouho	Jsetkouho
#define	ph_setkouho	Jph_setkouho
#define	hiraknj		Jhiraknj
#define	hiraknj_atrb	Jhiraknj_atrb
#define	hiraknj_ofs	Jhiraknj_ofs
#define	hiraknj_knj	Jhiraknj_knj
#define	hiraknj_hask	Jhiraknj_hask
#define	hiraknj_kask	Jhiraknj_kask
#define	hiraknj_norm	Jhiraknj_norm
#define	hiraknj_hira	Jhiraknj_hira



#define	getstb		Jgetstb
#define	setubi		Jsetubi



#define	sj2cd_chr	Jsj2cd_chr
#define	sj2cd_str	Jsj2cd_str



#define	retaiofs	Jretaiofs
#define	srchai2nd	Jsrchai2nd
#define	srchai1st	Jsrchai1st
#define	srchai4th	Jsrchai4th
#define	srchai3rd	Jsrchai3rd
#define	srchai6th	Jsrchai6th
#define	srchai5th	Jsrchai5th



#define	yomicmp_sub	Jyomicmp_sub
#define	yomicmp		Jyomicmp
#define	srchdict	Jsrchdict



#define	srchg_atrb	Jsrchg_atrb
#define	srchg_ofs	Jsrchg_ofs
#define	srchg_knj	Jsrchg_knj
#define	srchg_norm1	Jsrchg_norm1
#define	srchg_norm2	Jsrchg_norm2
#define	srchgrp2nd	Jsrchgrp2nd
#define	srchgrp1st	Jsrchgrp1st



#define	srchhead	Jsrchhead



#define	srchidx	Jsrchidx



#define	srch_josuu_sub	Jsrch_josuu_sub
#define	srch_josuu	Jsrch_josuu
#define	setnrec_sub	Jsetnrec_sub
#define	setnrec		Jsetnrec
#define	srchtbl		Jsrchtbl
#define	isconnect	Jisconnect
#define	string_cmp	Jstring_cmp
#define	check_num	Jcheck_num
#define	srch_number1	Jsrch_number1
#define	srch_kurai1	Jsrch_kurai1
#define	srch_kurai2	Jsrch_kurai2
#define	srch_number2	Jsrch_number2
#define	srchnum_sub	Jsrchnum_sub
#define	srchnum		Jsrchnum
#define	setwdnum	Jsetwdnum
#define	setucnum	Jsetucnum



#define	study		Jstudy
#define	srchstdy	Jsrchstdy



#define	terminate	Jterminate



#define	wakachi		Jwakachi




#define	cmpstr		Jcmpstr



#define	hzstrlen	Jhzstrlen



#define	istrcmp		Jistrcmp



#define	memcpy 		Jmemcpy 
#define	sstrncpy	Jsstrncpy



#define	memset		Jmemset



#define	mvmemd		Jmvmemd



#define	mvmemi		Jmvmemi



#define	skiphblk	Jskiphblk



#define	skipkstr	Jskipkstr



#define	sstrcmp		Jsstrcmp



#define	sstrlen		Jsstrlen



#define	sstrncmp	Jsstrncmp




#define	charsize	Jcharsize



#define	chrtbl		Jchrtbl



#define	conjadr		Jconjadr



#define	connadr		Jconnadr
#define	rigtadr		Jrigtadr



#define	cvtnum_func	Jcvtnum_func
#define	getkan_func	Jgetkan_func
#define	setjrec_func	Jsetjrec_func
#define	srchg_func	Jsrchg_func
#define	hiraknj_func	Jhiraknj_func



#define	fzkadr		Jfzkadr



#define	taipri		Jtaipri
#define	sttpri		Jsttpri



#define	kigou		Jkigou



#define	selsjadrs	Jselsjadrs



#define	stbadr		Jstbadr



#define	settou_ptr	Jsettou_ptr
#define	scncttbl	Jscncttbl



#define	suuji_tbl	Jsuuji_tbl
#define	kurai1_tbl	Jkurai1_tbl
#define	kurai2_tbl	Jkurai2_tbl
#define	num1tbl		Jnum1tbl
#define	num2tbl		Jnum2tbl
#define	num3tbl		Jnum3tbl
#define	num4tbl		Jnum4tbl
#define	num5tbl		Jnum5tbl
#define	num6tbl		Jnum6tbl



#define	termtbl		Jtermtbl

#endif
