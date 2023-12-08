# SPDX-License-Identifier: MIT
#
# Copyright (c) 2023  Masanori Ogino
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

.PHONY: all clean depend distclean install

include Makefile.configure

VMAJOR = 2
VMINOR = 0
VPATCH = 99
VERSION := $(VMAJOR).$(VMINOR).$(VPATCH)
LIBVER = 1

CFLAGS += -fPIC
CPPFLAGS += -I.
LDFLAGS += -L.

# Public API library
PUBLIC_LIBS = libsj3lib.a libsj3lib.so.$(LIBVER)
SJ3LIB_OBJS = level1.o sj3lib.o string.o

# Private libraries
PRIVATE_LIBS = libkanakan.a libsj3rkcv.a
KANAKAN_OBJS = adddic.o addelcmn.o alloc.o charsize.o chrtbl.o cl2knj.o \
               clstudy.o cmpstr.o conjunc.o connect.o cvtclknj.o cvtdict.o \
               cvtkanji.o deldic.o depend.o dict.o functbl.o fuzoku.o \
               fzkyomi.o getkanji.o getrank.o global2.o hzstrlen.o init.o \
               istrcmp.o memory2.o mk2claus.o mkbunset.o mkjiritu.o mkkouho.o \
               mknumber.o mvmemd.o mvmemi.o peepdic.o ph2knj.o ph_khtbl.o \
               priority.o prtytbl.o s2ctbl.o selclrec.o selsuuji.o setconj.o \
               setjrec.o setkouho.o setubi.o sj2code.o skiphblk.o skipkstr.o \
               srchdict.o srchhead.o srchidx.o srchnum.o stbtbl.o stttbl.o \
               study.o suujitbl.o terminat.o termtbl.o wakachi.o
SJ3RKCV_OBJS = rk_conv.o sj3_rkcv.o wc16_str.o

# Client applications
CLIENT_APPS   = sj3dic sj3mkdic sj3stat
SJ3_OBJS      = code.o common.o conv.o display.o douon.o edit.o etc.o \
                eucmessage.o funckey.o henkan.o kigou.o libif.o nmttyslot.o \
                romaji.o screen.o sj3.o sj3ver.o sjgetchar.o sjrc2.o \
                stat_conv.o term.o toroku.o version2.o
SJ3DIC_OBJS   = codecnv.o dictdisp.o dictmake.o hinsi.o sj3dic.o sj3err.o \
                sjrc.o
SJ3MKDIC_OBJS = char.o cnvhinsi.o file.o global.o hindo.o knjcvt.o makedict.o \
                makelist.o makeseg.o memory.o offset.o readline.o string2.o
SJ3STAT_OBJS  = sj3stat.o

# Server daemon
SJ3SERV_OBJS = comuni.o error.o execute.o main.o setup.o

ALL_OBJS = $(SJ3LIB_OBJS) $(KANAKAN_OBJS) $(SJ3RKCV_OBJS) $(SJ3_OBJS) \
           $(SJ3DIC_OBJS) $(SJ3MKDIC_OBJS) $(SJ3STAT_OBJS) $(SJ3SERV_OBJS)

all: $(PUBLIC_LIBS) $(PRIVATE_LIBS) $(CLIENT_APPS) sj3serv

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	mkdir -p $(DESTDIR)$(LIBDIR)
	mkdir -p $(DESTDIR)/etc/sj3
	mkdir -p $(DESTDIR)$(SHAREDIR)/sj3/dict
	$(INSTALL_PROGRAM) sj3dic $(DESTDIR)$(BINDIR)
	$(INSTALL_PROGRAM) sj3mkdic $(DESTDIR)$(BINDIR)
	$(INSTALL_PROGRAM) sj3serv $(DESTDIR)$(BINDIR)
	$(INSTALL_PROGRAM) sj3stat $(DESTDIR)$(BINDIR)
	$(INSTALL_LIB) libsj3lib.a $(DESTDIR)$(LIBDIR)
	$(INSTALL_LIB) libsj3lib.so.$(LIBVER) $(DESTDIR)$(LIBDIR)
	ln -sf libsj3lib.so.$(LIBVER) $(DESTDIR)$(LIBDIR)/libsj3lib.so
	$(INSTALL_DATA) sj3lib.h $(DESTDIR)$(INCLUDEDIR)
	$(INSTALL_DATA) serverrc $(DESTDIR)/etc/sj3
	./sj3mkdic dict/visual.eucjp $(DESTDIR)$(SHAREDIR)/sj3/dict/sj3main.dic > /dev/null

clean:
	rm -f $(PUBLIC_LIBS) $(PRIVATE_LIBS) $(ALL_OBJS) compats.o $(CLIENT_APPS) sj3serv

distclean: clean
	rm -f Makefile.configure Paths.h config.h config.log

libsj3lib.a: $(SJ3LIB_OBJS) compats.o
	$(AR) crs $@ $(SJ3LIB_OBJS) compats.o

libsj3lib.so.$(LIBVER): $(SJ3LIB_OBJS) compats.o
	$(CC) -shared -o $@ $(SJ3LIB_OBJS) compats.o $(LDFLAGS) -Wl,${LINKER_SONAME},$@ $(LDLIBS)

libkanakan.a: $(KANAKAN_OBJS) compats.o
	$(AR) crs $@ $(KANAKAN_OBJS) compats.o

libsj3rkcv.a: $(SJ3RKCV_OBJS) compats.o
	$(AR) crs $@ $(SJ3RKCV_OBJS) compats.o

sj3: $(SJ3_OBJS) libsj3lib.a libsj3rkcv.a compats.o
	$(CC) -static -o $@ $(SJ3_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -ltermcap -lsj3lib -lsj3rkcv

sj3dic: $(SJ3DIC_OBJS) libsj3lib.a compats.o
	$(CC) -static -o $@ $(SJ3DIC_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -lsj3lib

sj3mkdic: $(SJ3MKDIC_OBJS) libsj3lib.a compats.o
	$(CC) -static -o $@ $(SJ3MKDIC_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -lsj3lib

sj3serv: $(SJ3SERV_OBJS) libsj3lib.a libkanakan.a compats.o
	$(CC) -static -o $@ $(SJ3SERV_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -lsj3lib -lkanakan

sj3stat: $(SJ3STAT_OBJS) libsj3lib.a compats.o
	$(CC) -static -o $@ $(SJ3STAT_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -lsj3lib

Paths.h: Paths.h.in
	sed -e "s|@SJ3CONFDIR@|/etc/sj3|" -e "s|@SJ3DICTDIR@|$(SHAREDIR)/sj3/dict|" Paths.h.in > $@

depend:
	cc -E -MM -w *.c > Makefile.depend

include Makefile.depend
