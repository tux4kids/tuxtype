EXTRA_DIST = fingers.txt\
plants.txt\
words1.txt\
words2.txt\
words3.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words
	$(INSTALL_DATA) $(srcdir)/fingers.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/fingers.txt
	$(INSTALL_DATA) $(srcdir)/plants.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/plants.txt
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words1.txt
	$(INSTALL_DATA) $(srcdir)/words2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words2.txt
	$(INSTALL_DATA) $(srcdir)/words3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words3.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/fingers.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/plants.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/brazilian-portuguese/words/words3.txt
