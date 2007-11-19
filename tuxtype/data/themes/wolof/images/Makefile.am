
EXTRA_DIST = photo.png map.png

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/images
	$(INSTALL_DATA) $(srcdir)/photo.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/images
	$(INSTALL_DATA) $(srcdir)/map.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/images

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/images/photo.png
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/wolof/images/map.png
