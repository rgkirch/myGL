#!/usr/bin/env bash

cd ~/
sudo apt-get update
sude apt-get remove vim vim-runtime gvim vim-tiny vim-common vim-gui-common
sudo apt-get install -y git htop libatk1.0-dev libbonoboui2-dev libcairo2-dev libgnome2-dev libgnomeui-dev libgtk2.0-dev libncurses5-dev libx11-dev libxpm-dev libxt-dev python-dev ruby-dev curl wget zsh build-essential cmake python-dev python3-dev cmake tree linux-headers-$(uname -r) build-essential dkms exuberant-ctags doxygen
