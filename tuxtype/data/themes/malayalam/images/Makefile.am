
SUBDIRS = status keyboard

EXTRA_DIST = \
splat0.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images
	$(INSTALL_DATA) $(srcdir)/splat0.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/splat0.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/splat0.png
