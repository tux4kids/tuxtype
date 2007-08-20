
SUBDIRS = \
images\
words

EXTRA_DIST = keyboard.lst lang.po settings.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/keyboard.lst
	$(INSTALL_DATA) $(srcdir)/settings.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/settings.txt

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/russian/settings.txt
