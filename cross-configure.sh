#!/bin/sh

CONFIG_SHELL=/bin/sh
export CONFIG_SHELL
PREFIX=/usr/local/mingw
TARGET=i586-pc-mingw32
PATH="$PREFIX/bin:$PREFIX/$TARGET/bin:$PATH"
export PATH
if [ -f "$PREFIX/$TARGET/bin/$TARGET-sdl-config" ]; then
    SDL_CONFIG="$PREFIX/$TARGET/bin/$TARGET-sdl-config"
    export SDL_CONFIG
fi
cache=cross-config.cache
CPPFLAGS=-I$PREFIX/$TARGET/include LDFLAGS=-L$PREFIX/$TARGET/lib sh configure --cache-file="$cache" \
	--target=$TARGET --host=$TARGET --build=i386-linux --prefix="$PREFIX/$TARGET" \
	$*
status=$?
rm -f "$cache"
exit $status
