EXTRA_DIST = map.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/images/map.png



uninstall-local:
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/espanol/images/map.png
