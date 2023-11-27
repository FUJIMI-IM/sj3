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
.\"
.\"$SonyRCSfile: hinsi.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:46 $
.\"
.TL
\fBsj3serv\fP で使用される品詞
.RS
.LP
ここでは \fBsj3serv\fP で使用している品詞分けについて説明します。
.RE

.SH
＊＊注意点＊＊
.RS
.SH
上一段・下一段動詞の扱い
.RS
例えば、カ行上一段動詞 "起きる" の場合、
.RS
語幹は　　　　　"お"
.br
活用語尾は　　　"き、きる、きれ、きろ、きよ"
.RE
となります。
.br
活用語尾の１字目に注目すると、いずれの活用形においても共通なので、
便宜的に "おき" までを語幹として扱うことにします。
これによって "行" 及び "上下"の区別は必要なくなり、
ただ "一段動詞" だけの分類分けで済ませています。
.RE
.RE

.NH
名詞
.RS
.LP
固有名詞を除いた名詞です。
.br
これらは複合名詞を作りやすいか否かによって２つに分けられます。
.br
複合名詞を作りやすいか否かは、主観によって判断しています。
.RE
.NH 2
複合名詞を作りやすい名詞
.RS
.IP 名１(MEISI_1) \w'　　　　　　　　　　　　'u
他の分類に含まれない、複合名詞を作りやすい名詞
.IP 名２(MEISI_2)
接頭語 "お"
.IP 名３(MEISI_3)
接頭語 "ご"
.IP 名４(MEISI_4)
接尾語 "的" "化" のどちらか
.IP 名５(MEISI_5)
名詞＋サ変動詞、接頭語 "お"
.IP 名６(MEISI_6)
名詞＋サ変動詞
.IP 名７(MEISI_7)
名詞＋サ変動詞、接頭語 "ご"
.IP 名８(MEISI_8)
名詞＋形容動詞
.IP 名９(MEISI_9)
名詞＋形容動詞、接頭語 "お"
.IP 名１０(MEISI_10)
名詞＋形容動詞、接頭語 "ご"
.IP 名１１(MEISI_11)
名詞＋副詞
.RE
.RE
.NH 2
複合名詞を作りにくい名詞
.RS
.IP 代１(D_MEISI_1) \w'　　　　　　　　　　　　'u
他の分類に含まれない、複合名詞を作りにくい名詞
.IP 代２(D_MEISI_2)
接尾語 "さん"
.IP 代３(D_MEISI_3)
"〜をする"
.IP 代４(D_MEISI_4)
名詞＋サ変動詞
.IP 代５(D_MEISI_5)
名詞＋形容動詞
.IP 代６(D_MEISI_6)
名詞＋副詞
.RE
.RE

.NH
固有名詞
.RS
.IP 苗字(MYOUJI) \w'　　　　　　　　　　　　'u
人名の姓
.IP 名前(NAMAE)
人名の名
.IP 企業(KIGYOU)
企業名
.IP 地名(TIMEI)
都道府県・区郡・市町村 および主要都市の駅名
.IP 県区(KEN_KU)
都道府県・区の・・県(都,府,区)付き
.RE
.RE

.NH
その他体言
.RS
.IP 連体(RENTAISI) \w'　　　　　　　　　　　　'u
連体詞
.IP 接続(SETUZOKU)
接続詞
.IP 感動(KANDOUSI)
感動詞
.IP 助数(JOSUUSI1)
"第〜" が接続できる
.IP 助数２(JOSUUSI2)
"第〜”が接続できない
.IP 数詞(SUUSI)
数詞
.RE
.RE

.NH
接頭語
.RS
.IP 接頭１(SETTOU_1) \w'　　　　　　　　　　　　'u
名詞（サ変名詞・刑動名詞含む）に接続
.IP 接頭２(SETTOU_2)
地名に接続（北、南、西、東 等）
.IP 接頭３(SETTOU_3)
人名に接続（故 など）
.IP 接頭４(SETTOU_4)
数詞に接続（第、計 等）
.IP 接頭５(SETTOU_5)
接頭１ より接続が強い
.RE
.RE

.NH
接尾語
.RS
.IP 接尾１(SETUBI_1) \w'　　　　　　　　　　　　'u
名詞に接続、名詞扱い
.IP 接尾２(SETUBI_2)
名詞に接続、名詞扱い。ただし連濁形
.IP 接尾３(SETUBI_3)
接尾１ より接続が強い
.IP 接尾４(SETUBI_4)
地名に接続、名詞扱い
.IP 接尾５(SETUBI_5)
名詞に接続、形容詞扱い
.IP 接尾６(SETUBI_6)
名詞に接続、形容動詞扱い
.IP 接尾７(SETUBI_7)
名詞に接続、サ変名詞扱い
.IP 接尾８(SETUBI_8)
名詞に接続、一段動詞扱い
.IP 接尾９(SETUBI_9)
接尾語＋副詞
.RE
.RE

.NH
副詞
.RS
.IP 副１(FUKUSI_1) \w'　　　　　　　　　　　　'u
接続なし
.IP 副２(FUKUSI_2)
"〜と" 接続	
.IP 副３(FUKUSI_3)
"〜と", サ変動詞語尾が接続
.IP 副４(FUKUSI_4)
サ変動詞語尾が接続
.IP 副５(FUKUSI_5)
"〜に" 接続
.IP 副６(FUKUSI_6)
"〜も" 接続
.IP 副７(FUKUSI_7)
"〜の" 接続
.IP 副８(FUKUSI_8)
"〜と", "〜です、〜だ" 接続
.IP 副９(FUKUSI_9)
"〜と", サ変動詞語尾, "〜です、〜だ" 接続
.RE
.RE

.NH
形容詞
.RS
.RS
.DS
[さ]　　"さ" が接続して名詞化する
[げ]　　"げ" が接続して形容動詞化する
[め]　　"め" が接続して形容動詞化する
[み]　　"み" が接続して名詞化する
[がる]　"がる" が接続して動詞化する
.DE
.RE
.sp
.IP 形１(KEIYOUSI_1) \w'　　　　　　　　　　　　'u
上記付属語接続なし
.IP 形２(KEIYOUSI_2)
[さ]
.IP 形３(KEIYOUSI_3)
[さ、げ]
.IP 形４(KEIYOUSI_4)
[さ、がる]
.IP 形５(KEIYOUSI_5)
[さ、げ、がる]
.IP 形６(KEIYOUSI_6)
[さ、め、げ]
.IP 形７(KEIYOUSI_7)
[さ、め、げ、がる]
.IP 形８(KEIYOUSI_8)
[さ、み、め、げ、がる]
.IP 形９(KEIYOUSI_9)
接頭語 "お", [さ、げ、がる]
.IP 形１０(KEIYOUSI_10)
接頭語 "お", [さ、め、げ、がる]
.IP 形１１(KEIYOUSI_11)
[さ、み、め]
.RE
.RE

.NH
形容動詞
.RS
.IP 形動１(KE_DOUSI_1) \w'　　　　　　　　　　　　'u
ダ活用
.IP 形動２(KE_DOUSI_2)
タリ活用
.IP 形動３(KE_DOUSI_3)
ダ活用、語幹で連体修飾
.IP 形動４(KE_DOUSI_4)
ダ活用、"〜さ"
.IP 形動５(KE_DOUSI_5)
ダ活用、"〜さ"、語幹で連体修飾
.IP 形動６(KE_DOUSI_6)
ダ活用、接頭語 "お" が接続
.IP 形動７(KE_DOUSI_7)
ダ活用、"〜さ"、接頭語 "お" が接続
.IP 形動８(KE_DOUSI_8)
ダ活用、接頭語 "ご" が接続
.IP 形動９(KE_DOUSI_9)
ダ活用、"〜の"
.RE
.RE

.NH
動詞
.RS
.IP サ変(DO_SAHEN) \w'　　　　　　　　　　　　'u
サ変動詞、連用名詞扱いできない
.IP ザ変(DO_ZAHEN)
ザ変動詞。連用名詞扱いできない

.IP 一段１(DO_1DAN_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP 一段２(DO_1DAN_2)
連用名詞扱いできない、接頭語 "お"
.IP 一段３(DO_1DAN_3)
連用名詞扱いできる
.IP 一段４(DO_1DAN_4)
連用名詞扱いできる、接頭語 "お"

.IP カ五１(DO_KAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない、
.IP カ五２(DO_KAGO_2)
連用名詞扱いできない、接頭語 "お"
.IP カ五３(DO_KAGO_3)
連用名詞扱いできる
.IP カ五４(DO_KAGO_4)
連用名詞扱いできる、接頭語 "お"
.IP カ五５(DO_KAGO_5)
連用名詞扱いできない、[け、ける、けれ]
.IP カ五６(DO_KAGO_6)
連用名詞扱いできない、[け、ける、けれ]、接頭語 "お"
.IP カ五７(DO_KAGO_7)
連用名詞扱いできる、[け、ける、けれ]
.IP カ五８(DO_KAGO_8)
連用名詞扱いできる、[け、ける、けれ]、接頭語 "お"

.IP ガ五１(DO_GAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP ガ五２(DO_GAGO_2)
連用名詞扱いできない、接頭語 "お"
.IP ガ五３(DO_GAGO_3)
連用名詞扱いできる
.IP ガ五４(DO_GAGO_4)
連用名詞扱いできる、接頭語 "お"
.IP ガ五５(DO_GAGO_5)
連用名詞扱いできない、[げ、げる、げれ]
.IP ガ五６(DO_GAGO_6)
連用名詞扱いできない、[げ、げる、げれ]、接頭語 "お"
.IP ガ五７(DO_GAGO_7)
連用名詞扱いできる、[げ、げる、げれ]
.IP ガ五８(DO_GAGO_8)
連用名詞扱いできる、[げ、げる、げれ]、接頭語 "お"

.IP サ五１(DO_SAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP サ五２(DO_SAGO_2)
連用名詞扱いできない、接頭語 "お"
.IP サ五３(DO_SAGO_3)
連用名詞扱いできる
.IP サ五４(DO_SAGO_4)
連用名詞扱いできる、接頭語 "お"
.IP サ五５(DO_SAGO_5)
連用名詞扱いできない、[せ、せる、せれ]
.IP サ五６(DO_SAGO_6)
連用名詞扱いできない、[せ、せる、せれ]、接頭語 "お"
.IP サ五７(DO_SAGO_7)
連用名詞扱いできる、[せ、せる、せれ]
.IP サ五８(DO_SAGO_8)
連用名詞扱いできる、[せ、せる、せれ]、接頭語 "お"

.IP タ五１(DO_TAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP タ五２(DO_TAGO_2)
連用名詞扱いできない、接頭語 "お"
.IP タ五３(DO_TAGO_3)
連用名詞扱いできる
.IP タ五４(DO_TAGO_4)
連用名詞扱いできる、接頭語"お"
.IP タ五５(DO_TAGO_5)
連用名詞扱いできない、[て、てる、てれ]
.IP タ五６(DO_TAGO_6)
連用名詞扱いできない、[て、てる、てれ]、接頭語 "お"
.IP タ五７(DO_TAGO_7)
連用名詞扱いできる、[て、てる、てれ]
.IP タ五８(DO_TAGO_8)
連用名詞扱いできる、[て、てる、てれ]、接頭語 "お"

.IP ナ五(DO_NAGO) \w'　　　　　　　　　　　　'u
連用名詞扱いできない

.IP バ五１(DO_BAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP バ五２(DO_BAGO_2)
連用名詞扱いできない、"お"
.IP バ五３(DO_BAGO_3)
連用名詞扱いできる
.IP バ五４(DO_BAGO_4)
連用名詞扱いできる、"お"
.IP バ五５(DO_BAGO_5)
連用名詞扱いできない、[べ、べる、べれ]
.IP バ五６(DO_BAGO_6)
連用名詞扱いできない、[べ、べる、べれ]、接頭語 "お"
.IP バ五７(DO_BAGO_7)
連用名詞扱いできる、[べ、べる、べれ]
.IP バ五８(DO_BAGO_8)
連用名詞扱いできる、[べ、べる、べれ]、接頭語 "お"

.IP マ五１(DO_MAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない、
.IP マ五２(DO_MAGO_2)
連用名詞扱いできない、"お"
.IP マ五３(DO_MAGO_3)
連用名詞扱いできる
.IP マ五４(DO_MAGO_4)
連用名詞扱いできる、"お"
.IP マ五５(DO_MAGO_5)
連用名詞扱いできない、[め、める、めれ]
.IP マ五６(DO_MAGO_6)
連用名詞扱いできない、[め、める、めれ]、接頭語 "お"
.IP マ五７(DO_MAGO_7)
連用名詞扱いできる、[め、める、めれ]
.IP マ五８(DO_MAGO_8)
連用名詞扱いできる、[め、める、めれ]、接頭語 "お"

.IP ラ五１(DO_RAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP ラ五２(DO_RAGO_2)
連用名詞扱いできない、"お"
.IP ラ五３(DO_RAGO_3)
連用名詞扱いできる
.IP ラ五４(DO_RAGO_4)
連用名詞扱いできる、接頭語 "お"
.IP ラ五５(DO_RAGO_5)
連用名詞扱いできない、[れ、れる、れれ]
.IP ラ五６(DO_RAGO_6)
連用名詞扱いできない、[れ、れる、れれ]、接頭語 "お"
.IP ラ五７(DO_RAGO_7)
連用名詞扱いできる、[れ、れる、れれ]
.IP ラ五８(DO_RAGO_8)
連用名詞扱いできる、[れ、れる、れれ]、接頭語 "お"

.IP ワ五１(DO_WAGO_1) \w'　　　　　　　　　　　　'u
連用名詞扱いできない
.IP ワ五２(DO_WAGO_1)
連用名詞扱いできない、"お"
.IP ワ五３(DO_WAGO_1)
連用名詞扱いできる
.IP ワ五４(DO_WAGO_1)
連用名詞扱いできる、接頭語 "お"
.IP ワ五５(DO_WAGO_1)
連用名詞扱いできない、[え、える、えれ]
.IP ワ五６(DO_WAGO_1)
連用名詞扱いできない、[え、える、えれ]、接頭語 "お"
.IP ワ五７(DO_WAGO_1)
連用名詞扱いできる、[え、える、えれ]
.IP ワ五８(DO_WAGO_1)
連用名詞扱いできる、[え、える、えれ]、接頭語 "お"
.RE
.RE

.NH
特殊品詞
.RS
分類不可能な語、特殊な活用をする語句など
.sp
.IP 単漢(TANKANJI) \w'　　　　　　　　　　　　'u
単漢字
.sp
.IP サ変未１(SP_SA_MI1) \w'　　　　　　　　　　　　'u
さ
.IP サ変未２(SP_SA_MI2)
せ
.IP サ変未用(SP_SA_YOU)
し
.IP サ変終体(SP_SA_SYU)
する
.IP サ変仮(SP_SA_KAT)
すれ
.IP サ変命１(SP_SA_ME1)
しろ
.IP サ変命２(SP_SA_ME2)
せよ
.sp
.IP カ変未(SP_KA_MIZ)
こ
.IP カ変用(SP_KA_YOU)
き
.IP カ変終体(SP_KA_SYU)
くる
.IP カ変仮(SP_KA_KAT)
くれ
.IP カ変命(SP_KA_ME1)
こい
.sp
.IP 丁寧１(TEINEI1)
いらっしゃい、おっしゃい、ください、なさい
.IP 丁寧２(TEINEI2)
ござい
.sp
.IP カ五音便(SP_KA_ONB)
いっ
.sp
.IP 特殊副(SP_FUKUSI)
こう、そう、どう
.sp
.IP 挨拶(AISATU)
ありがとう、おめでとう、おはよう
.sp
.IP 特殊形容(SP_KEIYOUSI)
いい
.sp
.IP 一括(IKKATU) \w'　　　　　　　　　　　　'u
文節変換では参照されない語。"関連辞書" で関連を定義するために存在する語。
.RE
.RE
