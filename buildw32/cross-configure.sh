#!/bin/sh

# Script for crossbuilding of Windows binaries on Linux/Unix host
# using mingw port of GCC. Obtained from http://www.libsdl.org
# and lightly modified by David Bruce <davidstuartbruce@gmail.com>

CONFIG_SHELL=/bin/sh
export CONFIG_SHELL

# Set PREFIX, BUILD, TARGET, and CONFIG_PATH according to your build machine:

# Path to crossbuild setup:
#PREFIX=/usr/local/mingw
PREFIX=/usr/local/mingw-cross-env/usr

# Architecture of the build machine itself:
BUILD=x86_64-linux

# Architecture of the build machine's crossbuild setup
# as located by PREFIX:
TARGET=i686-pc-mingw32

# Path to configure script
CONFIG_PATH=../configure

PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
if [ -f "$PREFIX/$TARGET/bin/$TARGET-sdl-config" ]; then
    SDL_CONFIG="$PREFIX/$TARGET/bin/$TARGET-sdl-config"
    export SDL_CONFIG
fi

CPPFLAGS=-I$PREFIX/$TARGET/include LDFLAGS=-L$PREFIX/$TARGET/lib sh $CONFIG_PATH \
	--target=$TARGET --host=$TARGET --build=$BUILD --prefix="$PREFIX/$TARGET" \
	$*
status=$?

exit $status
