#!/bin/sh

if [ -f Makefile ]; then
    make clean
fi

rm -rf \
   CMakeFiles \
   _CPack_Packages \
   CPackConfig.cmake \
   CPackSourceConfig.cmake \
   Makefile \
   CMakeCache.txt \
   cmake_install.cmake \
   cmake_uninstall.cmake \
   install_manifest.txt \
   src/config.hpp
