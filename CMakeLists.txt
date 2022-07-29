#
#  Copyright (C) 2022 Dan Arrhenius <dan@ultramarin.se>
#
#  This file is part of macgen.
#
#  macgen is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
cmake_minimum_required (VERSION 3.15)

# Project name and info
#
project (macgen
    VERSION 1.0.5
    LANGUAGES CXX
    DESCRIPTION "Generate one or more random locally administered MAC addresses."
    HOMEPAGE_URL https://github.com/alfmep/macgen
    )


# Default GNU installation directories
#
include (GNUInstallDirs)


# Compiler flags
#
if (MSVC)
    add_compile_options (/DPROGRAM_NAME="macgen")
else()
    add_compile_options (-pipe -Wall -O2 -D_GNU_SOURCE)
endif()


# Program name and source files
#
add_executable (macgen
    src/appargs.cpp
    src/appargs.hpp
    src/main.cpp
    )
configure_file (src/config.hpp.in src/config.hpp)
# Compiler include search path
target_include_directories (macgen PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_BINARY_DIR}/src"
    )


# Installation
#
install (TARGETS macgen DESTINATION bin)
#
# Install man page
#
if (UNIX)
    install (
        FILES "${PROJECT_SOURCE_DIR}/src/macgen.1"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MANDIR}/man1")
endif()


# Uninstall target
#
if (NOT TARGET uninstall)
    configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target (uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()


# Add targets for distributable files
#
include (CPack)
