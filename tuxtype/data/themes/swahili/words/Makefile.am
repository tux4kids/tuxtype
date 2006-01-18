EXTRA_DIST = words1.txt


install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/words
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/words/words1.txt

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/swahili/words/words1.txt
