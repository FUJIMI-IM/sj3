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
