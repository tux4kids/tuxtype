EXTRA_DIST = \
planten.txt \
vingers.txt \
woorden1.txt \
woorden2.txt \
woorden3.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words
	$(INSTALL_DATA) $(srcdir)/planten.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/planten.txt
	$(INSTALL_DATA) $(srcdir)/vingers.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/vingers.txt
	$(INSTALL_DATA) $(srcdir)/woorden1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden1.txt
	$(INSTALL_DATA) $(srcdir)/woorden2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden2.txt
	$(INSTALL_DATA) $(srcdir)/woorden3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden3.txt

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/planten.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/vingers.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/nederlands/words/woorden3.txt
