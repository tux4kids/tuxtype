EXTRA_DIST = fingers.txt\
names.txt\
words1.txt\
words2.txt\
words3.txt\
words4.txt



install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words
	$(INSTALL_DATA) $(srcdir)/fingers.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/fingers.txt
	$(INSTALL_DATA) $(srcdir)/names.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/names.txt
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words1.txt
	$(INSTALL_DATA) $(srcdir)/words2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words2.txt
	$(INSTALL_DATA) $(srcdir)/words3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words3.txt
	$(INSTALL_DATA) $(srcdir)/words4.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words4.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/fingers.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/names.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words3.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/french/words/words4.txt
