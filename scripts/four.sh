#!/usr/bin/env bash

cd ~/
git clone https://github.com/rgkirch/dotfiles.git
ln -s dotfiles/.vimrc
vim +PluginInstall +qall
