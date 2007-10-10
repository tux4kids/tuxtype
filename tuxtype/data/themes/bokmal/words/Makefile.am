EXTRA_DIST = 3boks.txt\
4boks.txt\
5boks.txt\
6boks.txt\
dyrenavn.txt\
frukt.txt\
land.txt\
sprak.txt\
yrke.txt

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words
	$(INSTALL_DATA) $(srcdir)/3boks.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/3boks.txt
	$(INSTALL_DATA) $(srcdir)/4boks.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/4boks.txt
	$(INSTALL_DATA) $(srcdir)/5boks.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/5boks.txt
	$(INSTALL_DATA) $(srcdir)/6boks.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/6boks.txt
	$(INSTALL_DATA) $(srcdir)/dyrenavn.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/dyrenavn.txt
	$(INSTALL_DATA) $(srcdir)/frukt.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/frukt.txt
	$(INSTALL_DATA) $(srcdir)/land.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/land.txt
	$(INSTALL_DATA) $(srcdir)/sprak.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/sprak.txt
	$(INSTALL_DATA) $(srcdir)/yrke.txt $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/yrke.txt



uninstall-local:
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/3boks.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/4boks.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/5boks.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/6boks.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/dyrenavn.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/frukt.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/land.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/sprak.txt
	rm -f $(DESTDIR)$(prefix)/share/$(PACKAGE)/data/themes/bokmal/words/yrke.txt

