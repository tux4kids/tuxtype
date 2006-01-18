####### kdevelop will overwrite this part!!! (begin)##########

SUBDIRS = tuxtype 

EXTRA_DIST = tuxtype.kdevprj AUTHORS COPYING ChangeLog INSTALL README TODO tuxtype.lsm tuxtype.spec README.Win32 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/AUTHORS $(DESTDIR)$(docdir)/tuxtype/AUTHORS
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/COPYING $(DESTDIR)$(docdir)/tuxtype/COPYING
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/ChangeLog $(DESTDIR)$(docdir)/tuxtype/ChangeLog
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/INSTALL $(DESTDIR)$(docdir)/tuxtype/INSTALL
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/README $(DESTDIR)$(docdir)/tuxtype/README
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/TODO $(DESTDIR)$(docdir)/tuxtype/TODO
	$(mkinstalldirs) $(DESTDIR)$(docdir)/tuxtype/
	$(INSTALL_DATA) $(srcdir)/tuxtype.lsm $(DESTDIR)$(docdir)/tuxtype/tuxtype.lsm

uninstall-local:
	-rm -f $(DESTDIR)$(docdir)/tuxtype/AUTHORS
	-rm -f $(DESTDIR)$(docdir)/tuxtype/COPYING
	-rm -f $(DESTDIR)$(docdir)/tuxtype/ChangeLog
	-rm -f $(DESTDIR)$(docdir)/tuxtype/INSTALL
	-rm -f $(DESTDIR)$(docdir)/tuxtype/README
	-rm -f $(DESTDIR)$(docdir)/tuxtype/TODO
	-rm -f $(DESTDIR)$(docdir)/tuxtype/tuxtype.lsm

####### kdevelop will overwrite this part!!! (end)############
AUTOMAKE_OPTIONS = foreign
