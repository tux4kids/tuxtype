#!/bin/sh

# Script for crossbuilding of Windows binaries on Linux/Unix host
# using mingw port of GCC. Obtained from http://www.libsdl.org
# and lightly modified by David Bruce <davidstuartbruce@gmail.com>

CONFIG_SHELL=/bin/sh
export CONFIG_SHELL

# Set PREFIX, BUILD, TARGET, and CONFIG_PATH according to your build machine:

# Path to crossbuild setup:
# NOTE: this corresponds with a default mingw-cross-env location
# of "/opt/mingw-cross-env".  If mingw-cross-env is installed at
# e.g. "$HOME/mingw", set PREFIX to "$HOME/mingw/usr"
PREFIX=/opt/mingw-cross-env/usr

# NOTE: AFAICT, the key point seems to be that $BUILD is
# different from $TARGET to trigger "cross-compile" mode.
# This value appears to work even on 32-bit Linux
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
