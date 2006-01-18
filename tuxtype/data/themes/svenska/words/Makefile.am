EXTRA_DIST = ord1.txt\
ord2.txt\
ord3.txt



install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words
	$(INSTALL_DATA) $(srcdir)/ord1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord1.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words
	$(INSTALL_DATA) $(srcdir)/ord2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord2.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words
	$(INSTALL_DATA) $(srcdir)/ord3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord3.txt



uninstall-local:
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord1.txt
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord2.txt
	rm -f $(prefix)/share/$(PACKAGE)/data/themes/svenska/words/ord3.txt
