
EXTRA_DIST = \
map.png \
photo.png 

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/images/map.png
	$(INSTALL_DATA) $(srcdir)/photo.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/images/photo.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/images/map.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/euskara/images/photo.png
