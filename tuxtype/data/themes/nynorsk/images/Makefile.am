
SUBDIRS = status

EXTRA_DIST = splat0.png map.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/images
	$(INSTALL_DATA) $(srcdir)/splat0.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/images/splat0.png
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/images/map.png



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/images/splat0.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nynorsk/images/map.png
