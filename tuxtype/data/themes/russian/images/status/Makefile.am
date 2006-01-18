
EXTRA_DIST = \
score.png\
wave.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status
	$(INSTALL_DATA) $(srcdir)/score.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status/score.png
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status
	$(INSTALL_DATA) $(srcdir)/wave.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status/wave.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status/score.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/images/status/wave.png
