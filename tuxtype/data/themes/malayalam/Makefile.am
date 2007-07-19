
SUBDIRS = \
images\
words

EXTRA_DIST = keyboard.lst lang.po

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
