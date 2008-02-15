EXTRA_DIST = map.png photo.png

install-data-local:

	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/images
	$(INSTALL_DATA) $(srcdir)/photo.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/images



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/images/map.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/images/photo.png

