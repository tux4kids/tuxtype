EXTRA_DIST = map.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/espanol/images/map.png



uninstall-local:
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/espanol/images/map.png
