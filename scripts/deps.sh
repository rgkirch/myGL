#!/bin/sh
sudo apt-get install libglew-dev libglfw3-dev libglm-dev libboost-dev libboost-system-dev libboost-filesystem-dev libmagick++-dev
MODULES=$(find /usr/share/*cmake* -name 'FindGLEW.cmake' -printf '%h\n')
sudo cp -v ./cmakeModules/FindGLFW.cmake $MODULES

# note, ubuntu 14.04 and before does not have libglfw3-dev
# on account of that, this will fail. remove libglfw3-dev and try with libglfw-dev
