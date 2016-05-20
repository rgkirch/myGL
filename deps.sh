#!/bin/sh
sudo apt-get install libglew-dev libglfw3-dev libglm-dev libboost-dev
MODULES=$(find /usr/share/*cmake* -name 'FindGLEW.cmake' -printf '%h\n')
sudo cp -v ./cmakeModules/FindGLFW.cmake $MODULES

