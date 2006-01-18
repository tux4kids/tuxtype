# # Spec-file for TuxType version 1.0.3
#
Summary: An educational typing tutor game starring Tux
Name: tuxtype
Version: 1.0.3
Release: 1
Copyright: GPL
Group: Games/Educational
Source: http://www.tux4kids.org/dm/tuxtype/tuxtype-1.0.3.tar.gz
URL: http://www.tux4kids.org/dm/tuxtype/
Distribution: Tux4Kids <http://www.tux4kids.org>
Vendor: Tux4Kids <http://www.tux4kids.org>
Packager: Calvin Arndt <calarndt@tux4kids.org>
BuildRoot:/pkg

%description
Tux Typing is an educational typing tutor game starring Tux, the Linux
penguin. It is graphical and requires SDL to run. This is a stable
release.

%prep
rm -rf $RPM_BUILD_DIR/tuxtype-1.0.3
zcat $RPM_SOURCE_DIR/tuxtype-1.0.3.tar.gz | tar -xvf -

%build
cd tuxtype-1.0.3
./configure
make

%install
cd tuxtype-1.0.3
make DESTDIR=/pkg docdir=/usr/local/doc install

%files
/*

%changelog
* Mon Aug 19 2002 Sam "Criswell" Hart <criswell@geekcomix.com>
Wordfile loading fix. Docs updated. Prep for 1.0.3 release.

* Sat Aug 17 2002 Sam "Criswell" Hart <criswell@geekcomix.com>
Rand() fixes, smoothscroller patch, other performance and bugfixes.

* Thu Aug 01 2002 Sam "Criswell" Hart <criswell@geekcomix.com>
Performance & memleak patches from Calvin Arndt.

* Wed Aug 01 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Tweaked gameplay & menu speeds. Prep for 1.0 release

* Fri Jul 27 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Fixed word cascade LinuxPPC/Win32 crashing bug

* Sun Jul 15 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Added alpha blit option for fast word emphasis. Prepping for 1.0pre1 rel.

* Fri Jul 13 2001 Jesse Andrews <jdandr2@pop.uky.edu>
Fixed clipping blit bug as well as menu glitches

* Sat Jul 07 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Added preliminary theme support

* Tue Jun 20 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Added keyboard support to main menu

* Sat Jun 16 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Fixed tutorial screen crashing bug. Other minor work. Prepping for 0.9 rel.

* Fri Jun 15 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Better Kdevelop/SDL integration to fix non-POSIX OS support (first Win32 release).

* Tue Jun 12 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Further Tutorial stabilization work.

* Sat Jun 09 2001 Jesse Andrews <jdandr2@sheffield.cslab.uky.edu>
Fixed several small problems with Tutorial/Main screen bug.

* Sat May 19 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Fixed GIF->PNG and "yellow outline" in gameplay bugs.

* Mon May 14 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
Packaged up the 0.8 release.

* Fri May 04 2001 Sam "Criswell" Hart <criswell@geekcomix.com>
RPM ChangeLog began. For previous changes, see ChangeLog.old

