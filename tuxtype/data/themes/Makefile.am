
SUBDIRS = armenian\
bokmal\
brazilian-portuguese\
catalan\
czech\
dansk\
deutsch\
espanol\
euskara\
french\
greek\
italian\
jamaican\
lithuanian\
malayalam\
nederlands\
nynorsk\
russian\
svenska\
swahili \
wolof

EXTRA_DIST = lang.po

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes
	$(INSTALL_DATA) $(srcdir)/lang.po $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lang.po

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/lang.po
