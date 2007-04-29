
SUBDIRS = armenian\
bokmal\
brazilian-portuguese\
catalan\
dansk\
deutsch\
espanol\
euskara\
french\
greek\
italian\
jamaican\
lithuanian\
nederlands\
nynorsk\
russian\
svenska\
swahili

EXTRA_DIST = lang.po

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lang.po

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lang.po
