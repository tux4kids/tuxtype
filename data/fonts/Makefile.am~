## Process with Automake to create Makefile.in

## Support building with or without bundled fonts:
## (see also 'dist_with_fonts' target in top-level Makefile.am)
EXTRA_DIST = $(dist_fonts)


## The INSTALL_DATA lines are prefixed with '-' because the fonts may or may not be
## included (depending if package build with "make dist" or "make dist_with_fonts"
install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/AndikaDesRevG.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/DoulosSILR.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/lohit_bn.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/lohit_gu.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/lohit_hi.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/lohit_ta.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/Rachana_w01.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts
	-$(INSTALL_DATA) $(srcdir)/Vemana.ttf $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts

uninstall-local:
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/AndikaDesRevG.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/DoulosSILR.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/lohit_bn.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/lohit_gu.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/lohit_hi.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/lohit_ta.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Rachana_w01.ttf
	-rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/fonts/Vemana.ttf
