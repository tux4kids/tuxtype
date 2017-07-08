## Makefile.am for tuxtype - data/menus:
## Process with Automake to create Makefile.in

menusdir = $(pkgdatadir)/menus

dist_menus_DATA = main_menu.xml

EXTRA_DIST = menu_strings \
  update-po.sh


all: menu_strings

# Create a 'source' file holding all the descriptions for the missions.
menu_strings: $(dist_menus_DATA)
	cd $(srcdir); ./update-po.sh
