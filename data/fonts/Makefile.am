## Makefile.am for tuxtype - data/fonts:

## A little explanation - these variables allow building of the dist tarball
## either without the fonts ("make dist") or with the fonts ("make dist_with_fonts"),
## see also top level Makefile.am.
## 'data_fonts' is a list of the font files in this directory.
## 'dist_fonts' determines what fonts get bundled in. It is empty by default,
## but gets set to 'data_fonts' in the "make dist_with_fonts" target in the
## top-level Makefile.am.

EXTRA_DIST = $(dist_fonts)


## Handle install "manually" because otherwise it fails on package without fonts.
## The INSTALL_DATA lines are prefixed with '-' because the fonts may or may not be
## included (depending if package built with "make dist" or "make dist_with_fonts"
## 'fontsdir' is where the fonts are installed.

fontsdir = $(pkgdatadir)/fonts

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/AndikaDesRevG.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/DoulosSILR.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/Kedage-n.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/lohit_bn.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/lohit_gu.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/lohit_hi.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/lohit_ta.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/lohit_pa.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/Loma.ttf 		$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/Rachana_w01.ttf 	$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/utkal.ttf 		$(DESTDIR)$(fontsdir)
	-$(INSTALL_DATA) $(srcdir)/Vemana.ttf 		$(DESTDIR)$(fontsdir)

uninstall-local:
	-rm -f $(DESTDIR)$(fontsdir)/AndikaDesRevG.ttf
	-rm -f $(DESTDIR)$(fontsdir)/DoulosSILR.ttf
	-rm -f $(DESTDIR)$(fontsdir)/Kedage-n.ttf
	-rm -f $(DESTDIR)$(fontsdir)/lohit_bn.ttf
	-rm -f $(DESTDIR)$(fontsdir)/lohit_gu.ttf
	-rm -f $(DESTDIR)$(fontsdir)/lohit_hi.ttf
	-rm -f $(DESTDIR)$(fontsdir)/lohit_ta.ttf
	-rm -f $(DESTDIR)$(fontsdir)/lohit_pa.ttf
	-rm -f $(DESTDIR)$(fontsdir)/Loma.ttf
	-rm -f $(DESTDIR)$(fontsdir)/Rachana_w01.ttf
	-rm -f $(DESTDIR)$(fontsdir)/utkal.ttf
	-rm -f $(DESTDIR)$(fontsdir)/Vemana.ttf
	-rm -f $(DESTDIR)$(fontsdir)
