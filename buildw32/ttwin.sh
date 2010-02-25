#!/bin/sh

# An 'all-in-one' script to build the Windows installer executable.

./cross-make.sh clean
./cross-make.sh distclean
autoreconf --install ..
./cross-configure.sh --without-sdlpango --without-rsvg
./cross-make.sh
./cross-make.sh nsis

