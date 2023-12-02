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
��̾�����Ѵ������� sj3serv
.SH
�Ϥ����
.RS
\fBsj3serv\fP �ϡ����ˡ��ʳ��ˤǳ�ȯ���줿��̾�����Ѵ������ФǤ���
.br
���饤����ȤϤ��Υ����Ф��̿����뤳�Ȥˤ�ꡢ��̾�����Ѵ���Ԥ����Ȥ�
��ǽ�Ȥʤ�ޤ���
.RE

.NH
���Ѥ���ݡ���
.RS
.LP
sj3serv �� \fB/etc/servives\fP �Υ���ȥ� sj3serv �򻲾Ȥ��ơ��̿��ǻ��Ѥ���
�ݡ����ֹ��������ޤ���
.RE

.NH
���åȥ��åץե�����
.NH 2
�ե����ޥå�
.RS
.LP
���åȥ��åץե����������ϡ����ꤹ�뵡ǽ��Υ�����ɤȤ����ͤˤ��
�Ԥ��ޤ���
������ɤ��͡��ͤ��ͤζ��ڤ�ˤϥ��ڡ������뤤�ϥ��֤���Ѥ��ޤ���
������ɤ������륢��ե��٥åȤϡ���ʸ���Ⱦ�ʸ���ζ��̤�Ԥ��ޤ���
.br
�ƹԤ� '#' �ʹߤϥ����ȤȤ���̵�뤵��ޤ���'#' �򥳥��Ȥγ��ϵ���Ȥ��ʤ�����ˤ� '\\' ����Ԥ����ޤ���
.sp
.RS
.IP �㡧
.DS
MaxClient       32              # ���祯�饤����ȿ�
DictDir	        /var/dict/\e#dict
.DE
.RE
.RE
.RE
.sp
.NH 2
�������ǽ�ʵ�ǽ
.RS
.LP
���åȥ��åץե�����Ǥϡ��ʲ���ư������Ǥ��ޤ���
.RE
.NH 3
���祯�饤����ȿ�������
.RS
.LP
.RS
MaxClient       \fInumber_of_client\fP
.RE
.sp
�����С�����³���Ƥ褤���饤����Ȥο���10�ʿ��ǻ��ꤷ�ޤ���
���������������礭���ͤ����ꤷ�Ƥ⡢64��ۤ��륯�饤����Ȥ���
��³����뤳�ȤϤ���ޤ���
�ޤ��������ƥ�ǵ������������ۤ��ƥե����뤬�����ץ󤵤�뤳�Ȥ�
����ޤ��󤫤顢���ꤵ�줿������Υ��饤����Ȥ���³�Ǥ��ʤ����Ȥ⤢��ޤ���
.br
�ǥե���Ȥ�64�Ǥ���
.RE
.sp
.NH 3
����ǥ��쥯�ȥ������
.RS
.LP
.RS
DictDir         \fIdirectory_name\fP
.RE
.sp
�����¸�ߤ���ǥ��쥯�ȥ����ꤷ�ޤ���
.br
���饤����Ȥ�����ꤵ��뼭��ե�����̾��\'/\'�ǻϤޤ�Ȥ��ˤϡ�
���Υե�����̾�����Хѥ��Ȥ��Ƽ���˥����������ޤ�����\'/\'�ʳ��ΤȤ��ˤϡ�
�����ǻ��ꤵ���ǥ��쥯�ȥ꤫������Хѥ��Ȥ��Ƽ���˥����������ޤ���
.br
�ǥե���Ȥ� /usr/dict/sj3 �Ǥ���
.RE
.sp
.NH 3
����λ����꡼��
.RS
.LP
.RS
ReadDict        \fIdictionary_name\fP
.RE
.sp
�����Ф�Ω���夬������ɤ߹��༭�����ꤷ�ޤ���
.br
����ʼ���ϻ������ɤ߹���Ǥ������Ȥˤ�ꡢ���饤����Ȥ�Ω���夬����֤�û�̤��뤳�Ȥ�����ޤ���
.br
�������ɤ߹���Ǥ�������ϡ�ʣ�����ꤹ�뤳�Ȥ�����ޤ�����
���Ԥǻ���Ǥ���Τϣ��Ĥμ���̾�Ǥ���
.br
�ǥե���Ȥ��ɤ߹��ޤ�뼭��Ϥ���ޤ���
.RE
.sp
.NH 3
���顼���ϥե�����λ���
.RS
.LP
.RS
ErrorOut        \fIerror_out_filename\fP
.RE
.sp
���顼���˥󥰤���Ϥ���ե��������ꤷ�ޤ���
.br
�ǥե���Ȥ� /dev/null �Ǥ���
.RE
.sp
.NH 3
��³��ǽ�ʥ桼���λ���
.RS
.LP
.RS
AllowUser       \fIusername\fP \fIhostname\fP
.RE
.sp
�����Ф���³�Ǥ���桼�������¤��ޤ���
.br
���ιԤ���ꤹ�뤳�Ȥˤ�ꡢ�����Ф���³�Ǥ���Τϥۥ��� \fIhostname\fP ���
�桼�� \fIusername\fP ���������¤���ޤ���
.br
\fIhostname\fP �Ͼ�ά�Ǥ��ޤ��������ξ��ˤϥ桼��̾�� \fIusername\fP �Ǥ����
�ɤΥۥ��Ȥ���Ǥ�̵������³�Ǥ��뤳�Ȥˤʤ�ޤ���
\fIusername\fP��\fIhostname\fP �򵭽Ҥ���Ȥ��ˤϡ�ʣ��ʸ���ȥޥå��󥰤���
\'*\'�ȣ�ʸ���ȥޥå��󥰤���\'.\'��Ȥ����Ȥ�����ޤ���
.br
AllowUser ��ʣ���Խ񤯤��Ȥ�����ޤ���
.br
�ǥե���ȤǤ����ƤΥ桼������Ĥ��Ƥ��ޤ���
.RE
.sp