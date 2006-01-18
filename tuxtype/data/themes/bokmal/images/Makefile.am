
SUBDIRS = status

EXTRA_DIST = splat0.png map.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images
	$(INSTALL_DATA) $(srcdir)/splat0.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images/splat0.png
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images/map.png



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images/splat0.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/images/map.png
