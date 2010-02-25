#!/bin/sh

# Script for crossbuilding of Windows binaries on Linux/Unix host
# using mingw port of GCC. Obtained from http://www.libsdl.org
# and lightly modified by David Bruce <davidstuartbruce@gmail.com>

# Path to crossbuild setup:
#PREFIX=/usr/local/mingw
PREFIX=/usr/local/mingw-cross-env/usr

# Architecture of the build machine's crossbuild setup
# as located by PREFIX:
TARGET=i686-pc-mingw32

PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec make $*
