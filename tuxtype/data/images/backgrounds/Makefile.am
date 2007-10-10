
EXTRA_DIST = \
0.jpg\
1.jpg\
2.jpg\
3.jpg\
4.jpg

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds
	$(INSTALL_DATA) $(srcdir)/0.jpg $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/0.jpg
	$(INSTALL_DATA) $(srcdir)/1.jpg $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/1.jpg
	$(INSTALL_DATA) $(srcdir)/2.jpg $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/2.jpg
	$(INSTALL_DATA) $(srcdir)/3.jpg $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/3.jpg
	$(INSTALL_DATA) $(srcdir)/4.jpg $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/4.jpg

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/0.jpg
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/1.jpg
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/2.jpg
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/3.jpg
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/backgrounds/4.jpg
