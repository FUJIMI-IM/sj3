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
.\"$SonyRCSfile: sj3serv.ms,v $
.\"$SonyRevision: 1.1 $
.\"$SonyDate: 1994/06/03 08:00:49 $
.\"
.TL
仮名漢字変換サーバ sj3serv
.SH
はじめに
.RS
\fBsj3serv\fP は、ソニー（株）で開発された仮名漢字変換サーバです。
.br
クライアントはこのサーバと通信することにより、仮名漢字変換を行うことが
可能となります。
.RE

.NH
使用するポート
.RS
.LP
sj3serv は \fB/etc/servives\fP のエントリ sj3serv を参照して、通信で使用する
ポート番号を取得します。
.RE

.NH
セットアップファイル
.NH 2
フォーマット
.RS
.LP
セットアップファイルの設定は、設定する機能毎のキーワードとその値により
行います。
キーワードと値、値と値の区切りにはスペースあるいはタブを使用します。
キーワードを構成するアルファベットは、大文字と小文字の区別を行いません。
.br
各行の '#' 以降はコメントとして無視されます。'#' をコメントの開始記号としないためには '\\' を先行させます。
.sp
.RS
.IP 例：
.DS
MaxClient       32              # 最大クライアント数
DictDir	        /var/dict/\e#dict
.DE
.RE
.RE
.RE
.sp
.NH 2
再設定可能な機能
.RS
.LP
セットアップファイルでは、以下の動作を指定できます。
.RE
.NH 3
最大クライアント数の設定
.RS
.LP
.RS
MaxClient       \fInumber_of_client\fP
.RE
.sp
サーバーに接続してよいクライアントの数を10進数で指定します。
ただし、いくら大きな値を設定しても、64を越えるクライアントから
接続されることはありません。
また、システムで許される最大数を越えてファイルがオープンされることも
ありませんから、指定された最大数のクライアントが接続できないこともあります。
.br
デフォルトは64です。
.RE
.sp
.NH 3
辞書ディレクトリの設定
.RS
.LP
.RS
DictDir         \fIdirectory_name\fP
.RE
.sp
辞書の存在するディレクトリを指定します。
.br
クライアントから指定される辞書ファイル名が\'/\'で始まるときには、
そのファイル名を絶対パスとして辞書にアクセスしますが、\'/\'以外のときには、
ここで指定されるディレクトリからの相対パスとして辞書にアクセスします。
.br
デフォルトは /usr/local/lib/sj3/dict です。
.RE
.sp
.NH 3
辞書の事前リード
.RS
.LP
.RS
ReadDict        \fIdictionary_name\fP
.RE
.sp
サーバの立ち上がり時に読み込む辞書を指定します。
.br
巨大な辞書は事前に読み込んでおくことにより、クライアントの立ち上がる時間を短縮することが出来ます。
.br
事前に読み込んでおく辞書は、複数指定することが出来ますが、
１行で指定できるのは１つの辞書名です。
.br
デフォルトで読み込まれる辞書はありません。
.RE
.sp
.NH 3
エラー出力ファイルの指定
.RS
.LP
.RS
ErrorOut        \fIerror_out_filename\fP
.RE
.sp
エラーやワーニングを出力するファイルを指定します。
.br
デフォルトは /dev/null です。
.RE
.sp
.NH 3
接続可能なユーザの指定
.RS
.LP
.RS
AllowUser       \fIusername\fP \fIhostname\fP
.RE
.sp
サーバに接続できるユーザを制限します。
.br
この行を指定することにより、サーバに接続できるのはホスト \fIhostname\fP 上の
ユーザ \fIusername\fP だけに制限されます。
.br
\fIhostname\fP は省略できますが、その場合にはユーザ名が \fIusername\fP であれば
どのホストからでも無条件で接続できることになります。
\fIusername\fP、\fIhostname\fP を記述するときには、複数文字とマッチングする
\'*\'と１文字とマッチングする\'.\'を使うことが出来ます。
.br
AllowUser は複数行書くことが出来ます。
.br
デフォルトでは全てのユーザを許可しています。
.RE
.sp
