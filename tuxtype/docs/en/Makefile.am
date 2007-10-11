EXTRA_DIST = howtotheme.html

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/doc/$(PACKAGE)
	$(INSTALL_DATA) $(srcdir)/howtotheme.html $(DESTDIR)$(prefix)/doc/$(PACKAGE)/howtotheme.html

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/doc/$(PACKAGE)/howtotheme.html
