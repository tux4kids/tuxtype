EXTRA_DIST = map.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/images/map.png


uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/images/map.png
