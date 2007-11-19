
EXTRA_DIST = names.txt plants.txt words1.txt words2.txt words3.txt words4.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/names.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/plants.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/words1.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/words2.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/words3.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words
	$(INSTALL_DATA) $(srcdir)/words4.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/names.txt
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/plants.txt
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/words1.txt
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/words2.txt
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/words3.txt
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/words/words4.txt
