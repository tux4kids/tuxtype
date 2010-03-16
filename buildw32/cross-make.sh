#!/bin/sh

# Script for crossbuilding of Windows binaries on Linux/Unix host
# using mingw port of GCC. Obtained from http://www.libsdl.org
# and lightly modified by David Bruce <davidstuartbruce@gmail.com>

# Path to crossbuild setup:
# NOTE: this corresponds with a default mingw-cross-env location
# of "/opt/mingw-cross-env".  If mingw-cross-env is installed at
# e.g. "$HOME/mingw", set PREFIX to "$HOME/mingw/usr"
PREFIX=/opt/mingw-cross-env/usr

# Architecture of the build machine's crossbuild setup
# as located by PREFIX:
TARGET=i686-pc-mingw32

PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec make $*
