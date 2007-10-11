
SUBDIRS = status

EXTRA_DIST = \
splat0.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images
	$(INSTALL_DATA) $(srcdir)/splat0.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/splat0.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/splat0.png
