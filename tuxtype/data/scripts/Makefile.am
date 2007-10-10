
EXTRA_DIST = projectInfo.xml cascade.xml laser.xml Lesson_1.xml Lesson_2.xml Lesson_3.xml Lesson_4.xml

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts
	$(INSTALL_DATA) $(srcdir)/projectInfo.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/projectInfo.xml
	$(INSTALL_DATA) $(srcdir)/laser.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/laser.xml
	$(INSTALL_DATA) $(srcdir)/cascade.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/cascade.xml
	$(INSTALL_DATA) $(srcdir)/Lesson_1.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_1.xml
	$(INSTALL_DATA) $(srcdir)/Lesson_2.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_2.xml
	$(INSTALL_DATA) $(srcdir)/Lesson_3.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_3.xml
	$(INSTALL_DATA) $(srcdir)/Lesson_4.xml $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_4.xml

uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/projectInfo.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/laser.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/cascade.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_1.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_2.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_3.xml
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/scripts/Lesson_4.xml
