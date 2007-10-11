EXTRA_DIST = easy.txt



install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/greek/words
	$(INSTALL_DATA) $(srcdir)/easy.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/greek/words/easy.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/greek/words/easy.txt
