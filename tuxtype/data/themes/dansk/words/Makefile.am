EXTRA_DIST = words1.txt\
words2.txt\
words3.txt\
words4.txt



install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words1.txt
	$(INSTALL_DATA) $(srcdir)/words2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words2.txt
	$(INSTALL_DATA) $(srcdir)/words3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words3.txt
	$(INSTALL_DATA) $(srcdir)/words4.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words4.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words3.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/dansk/words/words4.txt
