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
.\"$SonyRCSfile: makedict.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:47 $
.\"
.TL
辞書生成ツール makedict
.RS
.LP
\fBmakedict\fP は、仮名漢字変換サーバ \fBsj3serv\fP で使用する辞書を
作成するためのツールです。
.sp
このツールを使用することにより、エディタで編集された辞書を \fBsj3serv\fP で
使用可能な形態に変換することが出来ます。
.sp
編集可能ファイルは、以下のような形式をしています。
.RS
.DS
あ　　　　　　　　　　　亜　　　　　　　　　　　接頭１:
あ　　　　　　　　　　　開　　　　　　　　　　　カ五２:
あ　　　　　　　　　　　空　　　　　　　　　　　カ五４:
あ　　　　　　　　　　　明　　　　　　　　　　　カ五２:
あ　　　　　　　　　　　飽　　　　　　　　　　　カ五３:
　　　　　　　　　　　　：
.DE
.RE
それぞれのフィールドは以下の意味を持ちます。
.RS
.DS
第１フィールド　　読み（全角ひらがな）
第２フィールド　　目的の出力
第３フィールド　　品詞の並び
.DE
.RE
ここで各フィールド間は、１つ以上のスペースあるいはタブで区切られている
必要があります。また各行は、読みでソートされていなければなりません。
.sp
第３フィールド（品詞のフィールド）には、別紙で定義されている品詞を記述します。
.sp
１つの熟語にたいして複数の品詞を指定するときには、以下のようにします。
.RS
.DS
あい　　　　　　　　　　愛　　　　　　　　　　　代４:サ五５:
あえ　　　　　　　　　　あえ　　　　　　　　　　ガ五３:一段１:
あお　　　　　　　　　　青　　　　　　　　　　　代１:形１１:
　　　　　　　　　　　　：
.DE
.RE
なおこの辞書では、体言（名詞・副詞・連体詞など非活用のもの）については
熟語そのままを、用言（動詞・形容詞・形容動詞など活用するもの）に
関しては語幹のみを登録しなければなりません。
.sp
このような編集可能ファイルから \fBsj3serv\fP で使用可能な辞書を生成するには、
\fBmakedict\fP を使用して以下のようにします。
.RS
.DS
makedict \fIsrc_file_name\fP \fIdst_file_name\fP
.DE
.RE
エラーがなく実行が終了したときには、\fIsrc_file_name\fP で指定された
編集可能ファイルが変換され、\fIdst_file_name\fP で指定されたファイルに
格納されています。
.RE
