
EXTRA_DIST = keyboard.png\
keyboard-us.png\
keyboard.eps\
keyboard-us.eps\
esc.png\
larrow.png\
space.png\
rarrow.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard
	$(INSTALL_DATA) $(srcdir)/keyboard.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard.png
	$(INSTALL_DATA) $(srcdir)/keyboard-us.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard-us.png
	$(INSTALL_DATA) $(srcdir)/keyboard.eps $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard.eps
	$(INSTALL_DATA) $(srcdir)/keyboard-us.eps $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard-us.eps
	$(INSTALL_DATA) $(srcdir)/esc.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/esc.png
	$(INSTALL_DATA) $(srcdir)/larrow.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/larrow.png
	$(INSTALL_DATA) $(srcdir)/space.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/space.png
	$(INSTALL_DATA) $(srcdir)/rarrow.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/rarrow.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard-us.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard.eps
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/keyboard-us.eps
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/esc.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/larrow.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/space.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/keyboard/rarrow.png
