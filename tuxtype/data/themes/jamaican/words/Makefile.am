EXTRA_DIST = jam-long.txt\
jam-med.txt\
jam-short.txt



install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words
	$(INSTALL_DATA) $(srcdir)/jam-long.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-long.txt
	$(INSTALL_DATA) $(srcdir)/jam-med.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-med.txt
	$(INSTALL_DATA) $(srcdir)/jam-short.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-short.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-long.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-med.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/jamaican/words/jam-short.txt
