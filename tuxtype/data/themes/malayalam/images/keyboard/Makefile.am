
EXTRA_DIST = \
key.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/keyboard
	$(INSTALL_DATA) $(srcdir)/key.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/keyboard/key.png
	
uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/malayalam/images/keyboard/key.png
	
