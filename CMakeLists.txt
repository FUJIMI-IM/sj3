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

cmake_minimum_required(VERSION 3.9)

project(sj3
    VERSION 2.0.95
    LANGUAGES C
)

include(GNUInstallDirs)

configure_file(
    "${CMAKE_SOURCE_DIR}/pathnames.h.in"
    "${CMAKE_BINARY_DIR}/pathnames.h"
)

include_directories("${CMAKE_BINARY_DIR}")

add_library(sj3lib
    lib/sj3lib/level1.c
    lib/sj3lib/sj.c
    lib/sj3lib/string.c
)
target_include_directories(sj3lib
    PRIVATE include
)

add_library(kanakan
    lib/kanakan/adddic.c
    lib/kanakan/addelcmn.c
    lib/kanakan/alloc.c
    lib/kanakan/charsize.c
    lib/kanakan/chrtbl.c
    lib/kanakan/cl2knj.c
    lib/kanakan/clstudy.c
    lib/kanakan/cmpstr.c
    lib/kanakan/conjunc.c
    lib/kanakan/connect.c
    lib/kanakan/cvtclknj.c
    lib/kanakan/cvtdict.c
    lib/kanakan/cvtkanji.c
    lib/kanakan/deldic.c
    lib/kanakan/depend.c
    lib/kanakan/dict.c
    lib/kanakan/functbl.c
    lib/kanakan/fuzoku.c
    lib/kanakan/fzkyomi.c
    lib/kanakan/getkanji.c
    lib/kanakan/getrank.c
    lib/kanakan/global.c
    lib/kanakan/hzstrlen.c
    lib/kanakan/init.c
    lib/kanakan/istrcmp.c
    lib/kanakan/memory.c
    lib/kanakan/mk2claus.c
    lib/kanakan/mkbunset.c
    lib/kanakan/mkjiritu.c
    lib/kanakan/mkkouho.c
    lib/kanakan/mknumber.c
    lib/kanakan/mvmemd.c
    lib/kanakan/mvmemi.c
    lib/kanakan/peepdic.c
    lib/kanakan/ph2knj.c
    lib/kanakan/ph_khtbl.c
    lib/kanakan/priority.c
    lib/kanakan/prtytbl.c
    lib/kanakan/s2ctbl.c
    lib/kanakan/selclrec.c
    lib/kanakan/selsuuji.c
    lib/kanakan/setconj.c
    lib/kanakan/setjrec.c
    lib/kanakan/setkouho.c
    lib/kanakan/setubi.c
    lib/kanakan/sj2code.c
    lib/kanakan/skiphblk.c
    lib/kanakan/skipkstr.c
    lib/kanakan/srchdict.c
    lib/kanakan/srchhead.c
    lib/kanakan/srchidx.c
    lib/kanakan/srchnum.c
    lib/kanakan/stbtbl.c
    lib/kanakan/stttbl.c
    lib/kanakan/study.c
    lib/kanakan/suujitbl.c
    lib/kanakan/terminat.c
    lib/kanakan/termtbl.c
    lib/kanakan/wakachi.c
)
target_include_directories(kanakan
    PRIVATE include
)

add_executable(sj3serv
    sj3serv/comuni.c
    sj3serv/error.c
    sj3serv/execute.c
    sj3serv/main.c
    sj3serv/setup.c
)
#add_executable(sj3serv
#    sj3serv/log.c
#    sj3serv/sj3serv.c
#)
target_include_directories(sj3serv
    PRIVATE include lib/sj3lib
)
target_link_libraries(sj3serv
    sj3lib kanakan
)

add_executable(sj3dic
    sj3dic/codecnv.c
    sj3dic/dictdisp.c
    sj3dic/dictmake.c
    sj3dic/hinsi.c
    sj3dic/sj3dic.c
    sj3dic/sj3err.c
    sj3dic/sjrc.c
)
target_include_directories(sj3dic
    PRIVATE include lib/sj3lib
)
target_link_libraries(sj3dic
    sj3lib
)

add_executable(sj3mkdic
    sj3mkdic/char.c
    sj3mkdic/cnvhinsi.c
    sj3mkdic/file.c
    sj3mkdic/global.c
    sj3mkdic/hindo.c
    sj3mkdic/knjcvt.c
    sj3mkdic/makedict.c
    sj3mkdic/makelist.c
    sj3mkdic/makeseg.c
    sj3mkdic/memory.c
    sj3mkdic/offset.c
    sj3mkdic/readline.c
    sj3mkdic/string.c
)
target_include_directories(sj3mkdic
    PRIVATE sj3mkdic include lib/sj3lib
)
target_link_libraries(sj3mkdic
    sj3lib
)
