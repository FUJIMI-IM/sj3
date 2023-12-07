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

.PHONY: all

include Makefile.configure

VMAJOR = 2
VMINOR = 0
VPATCH = 99
VERSION := $(VMAJOR).$(VMINOR).$(VPATCH)
LIBVER = 1

CFLAGS += -fPIC
CPPFLAGS += -I. -I./include

SJ3LIB_OBJS = level1.o sj3lib.o string.o

all: libsj3lib.a libsj3lib.so.$(LIBVER)

libsj3lib.a: $(SJ3LIB_OBJS) compats.o
	$(AR) rs $@ $(SJ3LIB_OBJS) compats.o

libsj3lib.so.$(LIBVER): $(SJ3LIB_OBJS) compats.o
	$(CC) -shared -o $@ $(SJ3LIB_OBJS) compats.o $(LDFLAGS) -Wl,${LINKER_SONAME},$@ $(LDLIBS)
	ln -sf $@ `basename $@ .$(LIBVER)`

Paths.h: Paths.h.in
	sed -e "s|@SJ3CONFDIR@|/etc/sj3|" -e "s|@SJ3DICTDIR@|$(SHAREDIR)/sj3/dict|" Paths.h.in > $@

$(SJ3LIB_OBJS) compats.o: config.h
$(SJ3LIB_OBJS): Paths.h
