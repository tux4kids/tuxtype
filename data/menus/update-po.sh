#!/bin/sh

# Create a 'source' file holding all the strings from the menus.
# That file (menu_strings) is listed in po/POTFILES.in so the strings will go into
# tuxmath.pot and subsequently into the individual po files for translation.
# NOTE this script needs to be kept in the same dir as the menu files

rm -f ./menu_strings
sed -n 's/^.*title=\("[^"]*"\).*$/_(\1)/p' main_menu.xml | sort | uniq > title_strings
sed -n 's/^.*desc=\("[^"]*"\).*$/_(\1)/p' main_menu.xml | sort | uniq > desc_strings
cat title_strings desc_strings > menu_strings
rm -f title_strings desc_strings
