.\"
.\"Copyright (c) 1991-1994  Sony Corporation
.\"
.\"Permission is hereby granted, free of charge, to any person obtaining
.\"a copy of this software and associated documentation files (the
.\""Software"), to deal in the Software without restriction, including
.\"without limitation the rights to use, copy, modify, merge, publish,
.\"distribute, sublicense, and/or sell copies of the Software, and to
.\"permit persons to whom the Software is furnished to do so, subject to
.\"the following conditions:
.\"
.\"The above copyright notice and this permission notice shall be
.\"included in all copies or substantial portions of the Software.
.\"
.\"THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
.\"EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
.\"MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
.\"IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
.\"DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
.\"OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
.\"THE USE OR OTHER DEALINGS IN THE SOFTWARE.
.\"
.\"Except as contained in this notice, the name of Sony Corporation
.\"shall not be used in advertising or otherwise to promote the sale, use
.\"or other dealings in this Software without prior written authorization
.\"from Sony Corporation.
.\"
.\"Author: Noriyuki Hosoe, Sony Corporation
.\"Modifier: Nobuyuki Tanaka, Sony Corporation
.\"
.\"$SonyRCSfile: sj3lib.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:48 $
.\"
.TL
sj3lib ライブラリ
.AB
.LP
\fBsj3lib\fP は、ソニー（株）で開発された仮名漢字変換サーバ \fIsj3serv\fP と
通信し仮名漢字変換を行う、クライアント用ライブラリです。
.sp
アプリケーションプログラムは、\fBsj3lib\fP を使用することにより
\&  \fIsj3serv\fP を使用した仮名漢字変換を行うことが可能となります。
.AE
.SH
はじめに
.RS
.IP ・
\fBsj3lib\fP では、関数や変数に sj3_ で始まる名前を使用していますので、
sj3_ で始まる関数名、変数名の使用はお避けください。
.IP ・
\fBsj3lib\fP で扱う文字コードは、関数名の最後に "_euc" がついているときは、
EUC です。"_mb" がついている場合は、引数の文字コードがプログラム実行時の
ロケールに依存します。なにも付いてないときには、引数の文字コードにシフト 
JIS を使用します。
.IP ・
メイン辞書、ユーザ辞書、学習ファイルは、接続するサーバの動作しているマシン上のものが使用されます。
.br
これらのファイルは、デフォルトではディレクトリ \fB/usr/local/lib/sj3/dict\fP の下におかれますが、サーバのセットアップファイルにより変更が可能です。
.IP ・
ユーザ辞書と学習ファイルは、関数 sj3_open で指定されたユーザ名を USER と
すれば、\fBuser/USER/private.dic\fP と \fBuser/USER/study.dat\fP です。
.RE
.NH
サーバの使用開始
.RS
.LP
.DS
sj3_open(hname, uname)
char	*hname;		/* ホスト名へのポインタ */
char	*uname;		/* ユーザ名へのポインタ */
.DE
.sp
文字列へのポインタ \fBhname\fP で指定されたホスト上のサーバ \fIsj3serv\fP に
対し、ユーザ名としてポインタ \fBuname\fP で指定される文字列を使用し、
接続を試みます。
.sp
接続には、ホスト名へのポインタ \fBhname\fP がナルポインタ、あるいは、
空の文字列または文字列 unix へのポインタの時には AF_UNIX ソケットを、
それ以外のときには AF_INET ソケットを使用します。
.sp
接続できたときには、サーバの立ち上げ時に指定された辞書ディレクトリ内にある
メイン辞書 sj3main.dic、ユーザ辞書 user/\fBuname\fP/private.dic、
学習ファイル user/\fBuname\fP/study.dat をオープンします。
.br
指定されたユーザ用のディレクトリ、ユーザ辞書、学習ファイルが
存在しないときには、それらの生成を試みます。
.sp
この関数は、かな漢字変換ルーチンを使用する以前に必ず呼ばなければなりません。
.sp
サーバと正常に接続できたときには 0 を、何かエラーのあったときには 0 以外の
エラーステータスを返します。
.sp
エラーステータスはヘッダファイル \fB/usr/local/include/sj3lib.h\fP で
定義されるビットアサインされた値で、詳細は以下のとおりです。
.sp
.IP "\fBSJ3_SERVER_DEAD\fP
接続処理中にサーバがダウンしました。
.IP "\fBSJ3_CONNECT_ERROR\fP
サーバがない、あるいは接続できません。
.IP "\fBSJ3_ALREADY_CONNECTED\fP
すでにサーバと接続済です。
.IP "\fBSJ3_CANNOT_OPEN_MDICT\fP
メイン辞書がオープンできません。
.IP "\fBSJ3_CANNOT_OPEN_UDICT\fP
ユーザ辞書がオープンできません。
.IP "\fBSJ3_CANNOT_OPEN_STUDY\fP
学習ファイルがオープンでいません。
.IP "\fBSJ3_CANNOT_MAKE_UDIR\fP
個人ファイルのためのディレクトリが作成できません。
.IP "\fBSJ3_CANNOT_MAKE_UDICT\fP
ユーザ辞書が作成できません。
.IP "\fBSJ3_CANNOT_MAKE_STUDY\fP
学習ファイルが作成できません。
.RE
.sp
エラーステータスには、これらのうち少なくとも 1 ビットが立っています。
.br
エラーステータスのうち \fBSJ3_SERVER_DEAD\fP あるいは \fBSJ3_CONNECT_ERROR\fP が
返されたとき以外は、サーバとの接続が確立されています。
.RE

.NH
サーバの使用終了
.RS
.LP
.DS
sj3_close()
.DE
.sp
\fIsj3_open\fP によって確立されたサーバとの接続を切り放します。
.br
メイン辞書、ユーザ辞書、学習ファイルのすべてがオープンされているものとして、
それらのクローズを試みます。
.sp
この関数は、かな漢字変換ルーチンの使用を終えるときに呼ばなければなりません。
.sp
サーバとの使用を正常に終了できたときには 0 を、
何かエラーのあったときには 0 以外のエラーステータスを返します。
.sp
エラーステータスはヘッダファイル \fB/usr/local/include/sj3lib.h\fP で
定義されるビットアサインされた値で、詳細は以下のとおりです。
.sp
.IP "\fBSJ3_SERVER_DEAD\fP
切り放し処理中にサーバがダウンしました。
.IP "\fBSJ3_DISCONNECT_ERROR\fP
サーバで内部エラーがありました。
.IP "\fBSJ3_NOT_CONNECTED\fP
サーバと接続されていません。
.IP "\fBSJ3_NOT_OPENED_MDICT\fP
メイン辞書がオープンされていません。
.IP "\fBSJ3_NOT_OPENED_UDICT\fP
ユーザ辞書がオープンされていません。
.IP "\fBSJ3_NOT_OPENED_STUDY\fP
学習ファイルがオープンされていません。
.IP "\fBSJ3_CLOSE_MDICT_ERROR\fP
メイン辞書がクローズできません。
.IP "\fBSJ3_CLOSE_UDICT_ERROR\fP
ユーザ辞書がクローズできません。
.IP "\fBSJ3_CLOSE_STUDY_ERROR\fP
学習ファイルがクローズできません。
.RE
.sp
エラーステータスには、これらのうち少なくとも 1 ビットが立っています。
.br
どのエラーステータスが返っても、サーバとの接続は切れています。
.RE

.NH
文章一括変換
.RS
.LP
.DS
sj3_getkan(yomi, bun, knjbuf, knjsiz)
sj3_getkan_euc(yomi, bun, knjbuf, knjsiz)
sj3_getkan_mb(yomi, bun, knjbuf, knjsiz)
unsigned char	*yomi;		/* 読み文字列へのポインタ */
struct bunsetu	bun[];		/* 文節構造体へのポインタ */
unsigned char	*knjbuf;	/* 漢字文字列格納領域へのポインタ */
int		knjsiz;		/* 漢字文字列格納領域の大きさ */
.DE
.sp
与えられた読みを文章一括変換し、その結果を返します。
.br
ポインタ \fByomi\fP で指定された読み文字列をかな漢字変換し、
ポインタ \fBknjbuf\fP で示される大きさ \fBknjsiz\fP の領域に
変換結果の漢字文字列を、配列 \fBstruct bunsetu bun[]\fP に
各文節に関する情報をセットします。
.br
\fBstruct bunsetu\fP はヘッダファイル \fB/usr/local/include/sj3lib.h\fP で
宣言されている以下のような構造体です。
.sp
.RS
.DS
struct bunsetu {
    int             srclen;         /* 読みの長さ         */
    int             destlen;        /* 漢字の長さ         */
    unsigned char   *srcstr;        /* 読みへのポインタ   */
    unsigned char   *deststr;       /* 漢字へのポインタ   */
    struct studyrec dcid;           /* 学習のためのデータ */
};
.DE
.RE
.sp
読みが漢字文字列に変換できたときには、これらの要素はきちんと設定されますが、
ポインタ \fBknjbuf\fP で示される領域の大きさ \fBbufsiz\fP が小さいなどの
原因で漢字文字列を返せない文節が生じたときには、その読み文字列への
ポインタを \fBsrcstr\fP に、またその長さを \fBsrclen\fP に持ち、
\fBdestlen\fP が 0 で \fBdeststr\fP がナルポインタの構造体が生成されます。
.sp
読みとして与えられる文字列は、
シフト JIS または EUC の 2 バイトコードで構成されていることが原則ですが、
1 バイトコードが混じっていてもかまいません。
ただしその際には、1 バイトコードは漢字に変換されることはありません。
.br
読み文字列はナルで終了していなければならず、
最大でも 256 文字を越えてはなりません。
.br
配列 \fBstruct bunsetu bun[]\fP の実体は、ユーザプログラム側で
取らなければなりません。
その大きさは最大で読み文字数分必要となります。
.br
変換時に必要とされるポインタ \fBknjbuf\fP で示される領域の大きさは、
変換する文字数や登録されている単語の長さなどによって
変化します。最低でも 1024 バイト程度はあったほうが良いでしょう。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
読みがナル文字列だった、あるいは読みが長すぎたときには 0 が返されます。
.br
正常終了したときには、配列 \fBstruct sj3bunsetu bun[]\fP にセットされた
文節数が返されます。
.RE

.NH
同音意義語数の取得
.RS
.LP
.DS
sj3_douoncnt(yomi)
sj3_douoncnt_euc(yomi)
sj3_douoncnt_mb(yomi)
unsigned char	*yomi;		/* 読み文字列へのポインタ */
.DE
.sp
与えられた読みを文節変換し、リターン値として同音意義語数を返します。
.sp
ポインタ \fByomi\fP によって読みとして与えられる文字列は、
シフト JIS または EUC の 2 バイトコードで構成されていることが原則ですが、
1 バイトコードが混じっていてもかまいません。
ただしその際には、1 バイトコードは漢字に変換されることはありません。
.br
読み文字列はナルで終了していなければならず、
最大でも 64 文字を越えてはなりません。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
読みがナル文字列だった、あるいは読みが長すぎた、または与えられた
読みで 1 文節が構成されなかったときには 0 が返されます。
.br
正常終了したときには同音語数が返されます。
.RE

.NH
同音意義語の取得
.RS
.LP
.DS
sj3_getdouon(yomi, dou)
sj3_getdouon_euc(yomi, dou)
sj3_getdouon_mb(yomi, dou)
unsigned char	*yomi;		/* 読み文字列へのポインタ */
struct douon	*dou;		/* 同音意義語構造体へのポインタ */
.DE
.sp
与えられた読みを文節変換し、同音意義語を与えられた構造体にセットす
るとともに、リターン値として同音意義語数を返します。
.sp
ポインタ \fByomi\fP によって読みとして与えられる文字列は、
シフト JIS または EUC の 2 バイトコードで構成されていることが原則ですが、
1 バイトコードが混じっていてもかまいません。
ただしその際には、1 バイトコードは漢字に変換されることはありません。
.br
読み文字列はナルで終了していなければならず、
最大でも 64 文字を越えてはなりません。
.sp
配列 \fBstruct douon dou[]\fP には同音意義語に関する情報がセットされます。
構造体 \fBstruct douon\fP は \fBsj3lib.h\fP で宣言されている
以下のような構造体です。
.sp
.RS
.DS
struct douon {
    unsigned char   ddata[512];	/* 同音意義語のデータ */
    int             dlen;		/* 同音意義語の長さ   */
    struct studyrec dcid;		/* 学習のためのデータ */
};
.DE
.RE
.sp
配列 \fBstruct douon dou[]\fP の実体はユーザプログラム側で
取らなければなりません。
.sp
構造体 \fBstruct douon\fP の \fBddata\fP は、ナルで終了する文字列です。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
読みがナル文字列だった、あるいは読みが長すぎた、または与えられた
読みで 1 文節が構成されなかったときには 0 が返されます。
.br
正常終了したときには、配列 \fBstruct douon dou[]\fP に
セットされた同音語数が返されます。
.RE

.NH
文節学習
.RS
.LP
.DS
sj_gakusyuu(dcid)
struct studyrec	*dcid;		/* 学習データへのポインタ */
.DE
.sp
学習データへのポインタ \fBdcid\fP の示す、
\fIsj3_getkan\fP や \fIsj_getdouon\fP により得られた学習データ
を使用して、文節学習を行います。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
正常に学習できたときには 0 が、学習できなかったときにはそれ以外の値が
返されます。
.RE

.NH
文節長学習
.RS
.LP
.DS
sj_gakusyuu2(yomi1, yomi2, dcid)
sj_gakusyuu2_euc(yomi1, yomi2, dcid)
sj_gakusyuu2_mb(yomi1, yomi2, dcid)
unsigned char	*yomi1;		/* 第１文節の読み文字列へのポインタ */
unsigned char	*yomi2;		/* 第２文節の読み文字列へのポインタ */
struct studyrec	*dcid;		/* 第２文節の学習データへのポインタ */
.DE
.sp
文節長学習を行います。
.sp
１文節として変換された文字列が２文節に分けられたような場合、
あるいは２文節の間でその境界が変更されたようなときには、
\fByomi1\fP に新たな第１文節の読みへのポインタを、
\fByomi2\fP に新たな第２文節の読みへのポインタを、
\fBdcid\fP に新たな第２文節の学習データを与えることにより、
文節長学習を行います。
.sp
２文節として変換された文字列が１文節になった場合には、
\fByomi1\fP に新たにできた１文節の読みへのポインタを、
\fByomi2\fP にはナルポインタまたはナル文字列へのポインタを与えることにより、
文節長学習を行います。このとき、\fBdcid\fP には設定すべきデータは
特にありません。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
正常に学習できたときには 0 が、学習できなかったときにはそれ以外の値が
返されます。
.RE

.NH
単語登録
.RS
.LP
.DS
sj3_touroku(yomi, kanji, hinsi)
sj3_touroku_euc(yomi, kanji, hinsi)
sj3_touroku_mb(yomi, kanji, hinsi)
unsigned char	*yomi;		/* 読み文字列へのポインタ */
unsigned char	*kanji;		/* 漢字文字列へのポインタ */
int	hinsi;			/* 品詞コード             */
.DE
.sp
読み文字列へのポインタ \fByomi\fP、
漢字文字列へのポインタ \fBkanji\fP、
品詞コード \fBhinsi\fP で指定された語をユーザ辞書に登録します。
.sp
読みは以下のシフト JIS または EUC コードで構成され、
かつナルで終了していなければなりません。
またその長さは、最大でも 31 文字を越えてはなりません。
.sp
.RS
.DS
ー＃＠〒
０１２３４ ５６７８９
ＡＢＣＤＥ ＦＧＨＩＪ ＫＬＭＮＯ ＰＱＲＳＴ ＵＶＷＸＹ Ｚ
ａｂｃｄｅ ｆｇｈｉｊ ｋｌｍｎｏ ｐｑｒｓｔ ｕｖｗｘｙ ｚ
あいうえお かきくけこ さしすせそ たちつてと なにぬねの
はひふへほ まみむめも や　ゆ　よ らりるれろ わゐ　ゑを ん
がぎぐげご ざじずぜぞ だぢづでど ばびぶべぼ ぱぴぷぺぽ
ぁぃぅぇぉ 　　っ　　 ゃ　ゅ　ょ ゎ
ヴヵヶ
.DE
.RE
.sp
ただし以下の文字は読みの先頭になることが出来ません。
.sp
.RS
.DS
ー を ぁぃぅぇぉ っ ゃゅょ ゎ ヵヶ
.DE
.RE
.sp
漢字はシフト JIS または EUC コードで構成され、かつナルで終了していなければ
なりません。
またその長さは、最大でも 32 文字を越えてはなりません。
.sp
品詞コードはヘッダファイル \fB/usr/local/include/sj3lib.h\fP で
定義される以下の品詞のうちの一つです。
.sp
.RS
.DS
SJ3_H_NRMNOUN   普通名詞        SJ3_H_ADJVERB   形容動詞
SJ3_H_PRONOUN   代名詞          SJ3_H_SILVERB   サ変動詞
SJ3_H_LNAME     苗字            SJ3_H_ZILVERB   ザ変動詞
SJ3_H_FNAME     名前            SJ3_H_ONEVERB   一段動詞
SJ3_H_LOCNAME   地名            SJ3_H_KAVERB    カ行五段動詞
SJ3_H_PREFIC    県／区名        SJ3_H_GAVERB    ガ行五段動詞
SJ3_H_RENTAI    連体詞          SJ3_H_SAVERB    サ行五段動詞
SJ3_H_CONJUNC   接続詞          SJ3_H_TAVERB    タ行五段動詞
SJ3_H_SUBNUM    助数詞          SJ3_H_NAVERB    ナ行五段動詞
SJ3_H_NUMERAL   数詞            SJ3_H_BAVERB    バ行五段動詞
SJ3_H_PREFIX    接頭語          SJ3_H_MAVERB    マ行五段動詞
SJ3_H_POSTFIX   接尾語          SJ3_H_RAVERB    ラ行五段動詞
SJ3_H_ADVERB    副詞            SJ3_H_WAVERB    ワ行五段動詞
SJ3_H_ADJECT    形容詞          SJ3_H_SINGLE    単漢字
.DE
.RE
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
登録できたときには 0 が、登録できなかったときにはそれ以外の
エラーステータスが返されます。
.sp
エラーステータスはヘッダファイル \fB/usr/local/include/sj3lib.h\fP で
定義され、詳細は以下のとおりです。
.sp
.IP "\fBSJ3_DICT_ERROR\fP
辞書が存在しない、あるいは登録できない辞書です。
.IP "\fBSJ3_DICT_LOCKED\fP
辞書が読み出しのためにロックされています。
.IP "\fBSJ3_BAD_YOMI_STR\fP
読み文字列が不正です。
.IP "\fBSJ3_BAD_KANJI_STR\fP
漢字文字列が不正です。
.IP "\fBSJ3_BAD_HINSI_CODE\fP
品詞コードが不正です。
.IP "\fBSJ3_WORD_EXIST\fP
すでに登録済です。
.IP "\fBSJ3_DOUON_FULL\fP
これ以上同音語を登録できません。
.IP "\fBSJ3_DICT_FULL\fP
これ以上辞書に登録できません。
.IP "\fBSJ3_INDEX_FULL\fP
これ以上インデックスに登録できません。
.IP "\fBSJ3_TOUROKU_FAILED\fP
他の原因で辞書への登録を失敗しました。
.RE
.RE

.NH
単語削除
.RS
.LP
.DS
sj3_syoukyo(yomi, kanji, hinsi)
sj3_syoukyo_euc(yomi, kanji, hinsi)
sj3_syoukyo_mb(yomi, kanji, hinsi)
unsigned char	*yomi;		/* 読み文字列へのポインタ */
unsigned char	*kanji;		/* 漢字文字列へのポインタ */
int	hinsi;			/* 品詞コード */
.DE
.sp
ユーザ辞書に登録されている
読み文字列へのポインタ \fByomi\fP、
漢字文字列へのポインタ \fBkanji\fP、
品詞コード \fBhinsi\fP で指定された語を消去します。
.sp
読みは以下のシフト JIS または EUC コードで構成され、
かつナルで終了していなければなりません。
またその長さは、最大でも 31 文字を越えてはなりません。
.sp
.RS
.DS
ー＃＠〒
０１２３４ ５６７８９
ＡＢＣＤＥ ＦＧＨＩＪ ＫＬＭＮＯ ＰＱＲＳＴ ＵＶＷＸＹ Ｚ
ａｂｃｄｅ ｆｇｈｉｊ ｋｌｍｎｏ ｐｑｒｓｔ ｕｖｗｘｙ ｚ
あいうえお かきくけこ さしすせそ たちつてと なにぬねの
はひふへほ まみむめも や　ゆ　よ らりるれろ わゐ　ゑを ん
がぎぐげご ざじずぜぞ だぢづでど ばびぶべぼ ぱぴぷぺぽ
ぁぃぅぇぉ 　　っ　　 ゃ　ゅ　ょ ゎ
ヴヵヶ
.DE
.RE
.sp
ただし以下の文字は読みの先頭になることが出来ません。
.sp
.RS
.DS
ー を ぁぃぅぇぉ っ ゃゅょ ゎ ヵヶ
.DE
.RE
.sp
漢字はシフト JIS または EUC コードで構成され、かつナルで終了していなければ
なりません。
またその長さは、最大でも 32 文字を越えてはなりません。
.sp
品詞コードは \fB/usr/local/include/sj3lib.h\fP で定義される以下の品詞の
うちの一つです。
.sp
.RS
.DS
SJ3_H_NRMNOUN   普通名詞        SJ3_H_ADJVERB   形容動詞
SJ3_H_PRONOUN   代名詞          SJ3_H_SILVERB   サ変動詞
SJ3_H_LNAME     苗字            SJ3_H_ZILVERB   ザ変動詞
SJ3_H_FNAME     名前            SJ3_H_ONEVERB   一段動詞
SJ3_H_LOCNAME   地名            SJ3_H_KAVERB    カ行五段動詞
SJ3_H_PREFIC    県／区名        SJ3_H_GAVERB    ガ行五段動詞
SJ3_H_RENTAI    連体詞          SJ3_H_SAVERB    サ行五段動詞
SJ3_H_CONJUNC   接続詞          SJ3_H_TAVERB    タ行五段動詞
SJ3_H_SUBNUM    助数詞          SJ3_H_NAVERB    ナ行五段動詞
SJ3_H_NUMERAL   数詞            SJ3_H_BAVERB    バ行五段動詞
SJ3_H_PREFIX    接頭語          SJ3_H_MAVERB    マ行五段動詞
SJ3_H_POSTFIX   接尾語          SJ3_H_RAVERB    ラ行五段動詞
SJ3_H_ADVERB    副詞            SJ3_H_WAVERB    ワ行五段動詞
SJ3_H_ADJECT    形容詞          SJ3_H_SINGLE    単漢字
.DE
.RE
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
削除できたときには 0 が、削除できなかったときにはそれ以外の
エラーステータスが返されます。
.sp
エラーステータスは \fB/usr/local/include/sj3lib.h\fP で定義され、
詳細は以下のとおりです。
.sp
.IP "\fBSJ3_DICT_ERROR\fP
辞書が存在しない、あるいは登録できない辞書です。
.IP "\fBSJ3_DICT_LOCKED\fP
辞書が読み出しのためにロックされています。
.IP "\fBSJ3_BAD_YOMI_STR\fP
読み文字列が不正です。
.IP "\fBSJ3_BAD_KANJI_STR\fP
漢字文字列が不正です。
.IP "\fBSJ3_BAD_HINSI_CODE\fP
品詞コードが不正です。
.IP "\fBSJ3_WORD_NOT_EXIST\fP
指定された語は登録されていません。
.IP "\fBSJ3_SYOUKYO_FAILED\fP
他の原因で辞書からの消去を失敗しました。
.RE
.RE

.NH
辞書のロック
.RS
.LP
.DS
sj3_lockserv()
.DE
.sp
現在オープンされている辞書に単語を登録／削除できないようにロックします。
.sp
学習ＩＤは単語の辞書内での位置を示しています。このため、クライアントが
学習ＩＤを保持している間に辞書登録／削除が行われると、クライアントの
保持している学習ＩＤと辞書の間で不整合を起こす可能性があります。
それを避けるために、クライアントが学習ＩＤを保持している間は辞書ファイルを
ロックする必要があります。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
ロックできたときには 0 が、
ロックできなかったときにはそれ以外の値が返されます。
.RE

.NH
辞書のアンロック
.RS
.LP
.DS
sj3_unlockserv()
.DE
.sp
sj3_lockserv 関数でロックされている辞書に単語を登録／削除できるように
アンロックします。
.sp
学習ＩＤは単語の辞書内での位置を示しています。このため、クライアントが
学習ＩＤを保持している間に辞書登録／削除が行われると、クライアントの
保持している学習ＩＤと辞書の間で不整合を起こす可能性があります。
それを避けるために、クライアントが学習ＩＤを保持している間は辞書ファイルを
ロックする必要があります。
.br
クライアントが辞書をロックしている間は、辞書登録／削除することができません。
クライアントは学習ＩＤを保持する必要がなくなったときには、ただちに辞書を
アンロックするべきです。
.sp
サーバと接続されていないとき、あるいはサーバがダウンしたときには \-1 が
返されます。
.br
アンロックできたときには 0 が、
アンロックできなかったときにはそれ以外の値が返されます。
.RE
