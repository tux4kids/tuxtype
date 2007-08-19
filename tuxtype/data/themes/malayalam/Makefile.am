
SUBDIRS = \
images\
words

EXTRA_DIST = keyboard.lst lang.po phrases.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/keyboard.lst $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam
	$(INSTALL_DATA) $(srcdir)/phrases.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/phrases.txt


uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/keyboard.lst
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/lang.po
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/phrases.txt

