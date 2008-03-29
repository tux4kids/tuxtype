#!/bin/sh

PREFIX=/usr/local/mingw
TARGET=i586-pc-mingw32
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
exec make $*
