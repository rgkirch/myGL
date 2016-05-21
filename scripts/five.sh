#!/usr/bin/env bash

cd ~/
cd ~/.vim/bundle/YouCompleteMe
./install.py --clang-completer
#  To simply compile with everything enabled, there's a --all flag. So, to install with all language features, ensure npm, go, mono, rust, and typescript API are installed and in your PATH, then simply run:
#    cd ~/.vim/bundle/YouCompleteMe
#    ./install.py --all
