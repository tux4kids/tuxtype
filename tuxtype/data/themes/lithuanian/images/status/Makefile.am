
EXTRA_DIST = gameover.png\
score.png\
wave.png

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status
	$(INSTALL_DATA) $(srcdir)/gameover.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/gameover.png
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status
	$(INSTALL_DATA) $(srcdir)/score.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/score.png
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status
	$(INSTALL_DATA) $(srcdir)/wave.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/wave.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/gameover.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/score.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lithuanian/images/status/wave.png
