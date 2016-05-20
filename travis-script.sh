#!/bin/sh
MODULES=$(find /usr/share/*cmake* -name 'FindGLEW.cmake' -printf '%h\n')
sudo cp -v ./cmakeModules/FindGLFW.cmake $MODULES

