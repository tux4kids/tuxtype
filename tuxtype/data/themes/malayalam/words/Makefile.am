EXTRA_DIST = words1.txt\
words2.txt\
words3.txt



install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words1.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words
	$(INSTALL_DATA) $(srcdir)/words2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words2.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words
	$(INSTALL_DATA) $(srcdir)/words3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words3.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/words/words3.txt
