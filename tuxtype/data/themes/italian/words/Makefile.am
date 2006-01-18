EXTRA_DIST = freq.txt word1.txt word2.txt word3.txt word4.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words
	$(INSTALL_DATA) $(srcdir)/freq.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/freq.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words
	$(INSTALL_DATA) $(srcdir)/word1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word1.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words
	$(INSTALL_DATA) $(srcdir)/word2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word2.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words
	$(INSTALL_DATA) $(srcdir)/word3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word3.txt
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words
	$(INSTALL_DATA) $(srcdir)/word4.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word4.txt


uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/freq.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word1.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word2.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word3.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/italian/words/word4.txt
