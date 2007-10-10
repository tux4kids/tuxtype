
EXTRA_DIST = gameover.png\
numbers.png\
score.png\
tux_helmet1.png\
tux_helmet2.png\
tux_helmet3.png\
wave.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status
	$(INSTALL_DATA) $(srcdir)/gameover.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/gameover.png
	$(INSTALL_DATA) $(srcdir)/numbers.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/numbers.png
	$(INSTALL_DATA) $(srcdir)/score.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/score.png
	$(INSTALL_DATA) $(srcdir)/tux_helmet1.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet1.png
	$(INSTALL_DATA) $(srcdir)/tux_helmet2.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet2.png
	$(INSTALL_DATA) $(srcdir)/tux_helmet3.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet3.png
	$(INSTALL_DATA) $(srcdir)/wave.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/wave.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/gameover.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/numbers.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/score.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet1.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet2.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/tux_helmet3.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/status/wave.png
