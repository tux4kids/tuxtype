#!/bin/sh

# An 'all-in-one' script to build the Windows installer executable.
# NOTE we have to use sudo because "make install" has a chown root command

echo "Cleaning build directory:"
sudo ./cross-make.sh clean>/dev/null
sudo ./cross-make.sh distclean>/dev/null
echo "Running autoreconf --install .."
autoreconf --install ..
echo "Running ./cross-configure.sh --host=i686-pc-mingw32"
./cross-configure.sh --host=i686-pc-mingw32 --without-rsvg --without-sdlnet
echo "Running ./cross-make.sh"
./cross-make.sh
echo "Running ./cross-make.sh dist (to generate gmo files)"
./cross-make.sh dist
echo "Building NSIS installer file"
sudo ./cross-make.sh nsis

