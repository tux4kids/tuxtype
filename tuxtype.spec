# Spec-file for TuxType version 1.5.10

%define name  tuxtype
%define version 1.5.10
%define release 1

Summary: An educational typing tutor game starring Tux
Name: %{name}
Version: %{version}
Release: %{release}
Source: http://www.geekcomix.com/dm/tuxtype/%{name}-%{version}.tar.gz
URL: http://www.geekcomix.com/dm/tuxtype/
License: GPL 
Group: Games/Educational
Distribution: Digital Monkey Software <http://www.geekcomix.com/dm/>
Vendor: Digital Monkey Software <http://www.geekcomix.com/dm/>
Packager: Calvin Arndt <calarndt@yahoo.com>
BuildRoot: %{tmp}
Prefix: $prefix

%description 
Tux Typing is an educational typing tutor game starring Tux, the Linux
penguin. It is graphical and requires SDL to run. This is a stable
release.

%prep

%setup -q -n %{name}-%{version}

%build

if test x%{prefix} = x; then
./configure --prefix="/usr/local"
else
./configure --prefix=%{prefix}
fi
   
make

%install

if test x%{prefix} = x; then

make prefix=$RPM_BUILD_ROOT/usr/local install

else

make prefix=$RPM_BUILD_ROOT%{prefix} install

fi


%files
%docdir  %{prefix}/doc/tuxtype
/*

%clean

rm -rf $RPM_BUILD_DIR/%{name}-%{version}
rm -rf $RPM_BUILD_ROOT%{prefix}

%changelog
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

