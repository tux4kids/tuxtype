#!/bin/bash
# This is a quick kludge to get the x,y coordinates right in the xml scripts
# this file resets the coordinates in case you previously built a windows distribution
# both files may be removed once SDL_Pango is working in the windows cross-build

echo -e "Editing lesson scripts for a non-windows build...\n"

touch fixed-linux

sed -f sed-linux.rules -i *

# Don't break the build if something went wrong!
exit 0

