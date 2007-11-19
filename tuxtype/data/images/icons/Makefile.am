EXTRA_DIST = tuxtype.ico

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/icons
	$(INSTALL_DATA) $(srcdir)/tuxtype.ico $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/icons/tuxtype.ico

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/icons/tuxtype.ico
