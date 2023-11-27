# SJ3

## Summary

SJ3 is a kana-kanji conversion system for Japanese text input.

## License

Unless otherwise noted, SJ3 is released under the MIT License.

## Installation

NB: Ugly hacks here and there...

```
cmake -B build -DCMAKE_C_FLAGS="-w"
cmake --build build
./build/sj3mkdic dict/visual.dic dict/sj3main.dic > /dev/null
sudo cp ./build/sj3serv /usr/local/bin/
sudo mkdir -p /usr/local/share/sj3/dict
sudo cp ./dict/sj3main.dic /usr/local/share/sj3/dict/
sudo mkdir -p /usr/local/lib/sj3
sudo cp ./server/serverrc /usr/local/lib/sj3/
sudo uim-module-manager --register sj3
uim-pref-gtk3
# Enable SJ3
# Modify socket path to /tmp/sj3sock
# Press OK
# Restart X session
```

## Acknowledgments

We highly appreciate the release of SJ3 source code as a free and open source
software by Sony Corporation.

<!--
  -- Copyright (c) 2023 Masanori Ogino <mogino@acm.org>
  -- SPDX-License-Identifier: MIT
  -->
