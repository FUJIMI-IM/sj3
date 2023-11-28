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
    VERSION 2.0.92
    LANGUAGES C
)

add_library(sj3lib
    sj3lib/level1.c
    sj3lib/sj.c
    sj3lib/string.c
)
target_include_directories(sj3lib
    PRIVATE include
)

add_library(sj3rkcv
    sj3rkcv/rk_conv.c
    sj3rkcv/sj3_rkcv.c
    sj3rkcv/wc16_str.c
)
target_include_directories(sj3rkcv
    PRIVATE include sj3h
)

add_library(kanakan
    kanakan/adddic.c
    kanakan/addelcmn.c
    kanakan/alloc.c
    kanakan/charsize.c
    kanakan/chrtbl.c
    kanakan/cl2knj.c
    kanakan/clstudy.c
    kanakan/cmpstr.c
    kanakan/conjunc.c
    kanakan/connect.c
    kanakan/cvtclknj.c
    kanakan/cvtdict.c
    kanakan/cvtkanji.c
    kanakan/deldic.c
    kanakan/depend.c
    kanakan/dict.c
    kanakan/functbl.c
    kanakan/fuzoku.c
    kanakan/fzkyomi.c
    kanakan/getkanji.c
    kanakan/getrank.c
    kanakan/global.c
    kanakan/hzstrlen.c
    kanakan/init.c
    kanakan/istrcmp.c
    kanakan/memcpy.c
    kanakan/memory.c
    kanakan/memset.c
    kanakan/mk2claus.c
    kanakan/mkbunset.c
    kanakan/mkjiritu.c
    kanakan/mkkouho.c
    kanakan/mknumber.c
    kanakan/mvmemd.c
    kanakan/mvmemi.c
    kanakan/peepdic.c
    kanakan/ph2knj.c
    kanakan/ph_khtbl.c
    kanakan/priority.c
    kanakan/prtytbl.c
    kanakan/s2ctbl.c
    kanakan/selclrec.c
    kanakan/selsuuji.c
    kanakan/setconj.c
    kanakan/setjrec.c
    kanakan/setkouho.c
    kanakan/setubi.c
    kanakan/sj2code.c
    kanakan/skiphblk.c
    kanakan/skipkstr.c
    kanakan/srchdict.c
    kanakan/srchhead.c
    kanakan/srchidx.c
    kanakan/srchnum.c
    kanakan/sstrcmp.c
    kanakan/sstrlen.c
    kanakan/sstrncmp.c
    kanakan/stbtbl.c
    kanakan/stttbl.c
    kanakan/study.c
    kanakan/suujitbl.c
    kanakan/terminat.c
    kanakan/termtbl.c
    kanakan/wakachi.c
)
target_include_directories(kanakan
    PRIVATE include
)

add_executable(sj3stat
    sj3stat/sj3stat.c
)
target_include_directories(sj3stat
    PRIVATE include sj3lib
)
target_link_libraries(sj3stat
    sj3lib
)

add_executable(sj3serv
    sj3serv/comuni.c
    sj3serv/error.c
    sj3serv/execute.c
    sj3serv/main.c
    sj3serv/setup.c
    sj3serv/time_stamp.c
    sj3serv/version.c
)
target_include_directories(sj3serv
    PRIVATE include sj3lib
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
    PRIVATE include sj3lib
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
    PRIVATE sj3mkdic include sj3lib
)
target_link_libraries(sj3mkdic
    sj3lib
)
