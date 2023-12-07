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

.PHONY: all install

include Makefile.configure

VMAJOR = 2
VMINOR = 0
VPATCH = 99
VERSION := $(VMAJOR).$(VMINOR).$(VPATCH)
LIBVER = 1

CFLAGS += -fPIC
CPPFLAGS += -I. -I./include
LDFLAGS += -L.

SJ3LIB_OBJS = level1.o sj3lib.o string.o
SJ3STAT_OBJS = sj3stat.o

all: libsj3lib.a libsj3lib.so.$(LIBVER) sj3stat

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	mkdir -p $(DESTDIR)$(LIBDIR)
	$(INSTALL_PROGRAM) sj3stat $(DESTDIR)$(BINDIR)
	$(INSTALL_LIB) libsj3lib.a $(DESTDIR)$(LIBDIR)
	$(INSTALL_LIB) libsj3lib.so $(DESTDIR)$(LIBDIR)
	$(INSTALL_LIB) libsj3lib.so.$(LIBVER) $(DESTDIR)$(LIBDIR)
	$(INSTALL_DATA) sj3lib.h $(DESTDIR)$(INCLUDEDIR)

libsj3lib.a: $(SJ3LIB_OBJS) compats.o
	$(AR) rs $@ $(SJ3LIB_OBJS) compats.o

libsj3lib.so.$(LIBVER): $(SJ3LIB_OBJS) compats.o
	$(CC) -shared -o $@ $(SJ3LIB_OBJS) compats.o $(LDFLAGS) -Wl,${LINKER_SONAME},$@ $(LDLIBS)
	ln -sf $@ `basename $@ .$(LIBVER)`

sj3stat: $(SJ3STAT_OBJS) libsj3lib.a compats.o
	$(CC) -static -o $@ $(SJ3STAT_OBJS) compats.o $(LDFLAGS) $(LDLIBS) -lsj3lib

Paths.h: Paths.h.in
	sed -e "s|@SJ3CONFDIR@|/etc/sj3|" -e "s|@SJ3DICTDIR@|$(SHAREDIR)/sj3/dict|" Paths.h.in > $@

$(SJ3LIB_OBJS) $(SJ3STAT_OBJS) compats.o: config.h
$(SJ3LIB_OBJS) $(SJ3STAT_OBJS): Paths.h