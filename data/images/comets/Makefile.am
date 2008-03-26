EXTRA_DIST = comet1.png\
comet2.png\
comet3.png\
cometex1.png\
cometex2.png



install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets
	$(INSTALL_DATA) $(srcdir)/comet1.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet1.png
	$(INSTALL_DATA) $(srcdir)/comet2.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet2.png
	$(INSTALL_DATA) $(srcdir)/comet3.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet3.png
	$(INSTALL_DATA) $(srcdir)/cometex1.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/cometex1.png
	$(INSTALL_DATA) $(srcdir)/cometex2.png $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/cometex2.png



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet1.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet2.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/comet3.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/cometex1.png
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/images/comets/cometex2.png
