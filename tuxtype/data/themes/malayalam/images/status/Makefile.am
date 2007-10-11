
EXTRA_DIST = \
score.png\
wave.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/status
	$(INSTALL_DATA) $(srcdir)/score.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/status/score.png
	$(INSTALL_DATA) $(srcdir)/wave.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/status/wave.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/status/score.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/status/wave.png
