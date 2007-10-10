
EXTRA_DIST = \
splat0.png \
title1.png 

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/images
	$(INSTALL_DATA) $(srcdir)/splat0.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/images/splat0.png
	$(INSTALL_DATA) $(srcdir)/title1.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/images/title1.png

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/images/splat0.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/armenian/images/title1.png
