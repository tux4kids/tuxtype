EXTRA_DIST = kurz2-3-mit.txt\
kurz2-3.txt\
lang7-8-mit.txt\
lang7-8.txt\
mittel4-6-mit.txt\
mittel4-6.txt




install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words
	$(INSTALL_DATA) $(srcdir)/kurz2-3-mit.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/kurz2-3-mit.txt
	$(INSTALL_DATA) $(srcdir)/kurz2-3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/kurz2-3.txt
	$(INSTALL_DATA) $(srcdir)/lang7-8-mit.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/lang7-8-mit.txt
	$(INSTALL_DATA) $(srcdir)/lang7-8.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/lang7-8.txt
	$(INSTALL_DATA) $(srcdir)/mittel4-6-mit.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/mittel4-6-mit.txt
	$(INSTALL_DATA) $(srcdir)/mittel4-6.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/mittel4-6.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/kurz2-3-mit.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/kurz2-3.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/lang7-8-mit.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/lang7-8.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/mittel4-6-mit.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/deutsch/words/mittel4-6.txt



