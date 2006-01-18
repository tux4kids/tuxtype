
EXTRA_DIST = \
letters.ttf \
menu.ttf 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts
	$(INSTALL_DATA) $(srcdir)/letters.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts/letters.ttf
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts
	$(INSTALL_DATA) $(srcdir)/menu.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts/menu.ttf

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts/letters.ttf
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/fonts/menu.ttf
