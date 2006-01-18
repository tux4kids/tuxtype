EXTRA_DIST = excuseme.wav



install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/sounds
	$(INSTALL_DATA) $(srcdir)/excuseme.wav $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/sounds/excuseme.wav



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/sounds/excuseme.wav
