#!/bin/sh

if [ -f Makefile ]; then
    make clean
fi

rm -rf \
   CMakeFiles \
   Makefile \
   CMakeCache.txt \
   cmake_install.cmake \
   cmake_uninstall.cmake \
   install_manifest.txt \
   src/config.h
