#!/bin/bash
# This is a quick kludge to get the x,y coordinates right in the xml scripts
# it can be removed once SDL_Pango is working in the windows crossbuild

echo -e "Editing lesson scripts for a windows build...\n"

sed -f sed-windows.rules -i *

# Don't break the build if something went wrong!
exit 0

