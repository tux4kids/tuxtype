## Makefile.am for tuxtype - data/scripts:
## Process with Automake to create Makefile.in

scripts_brailledir = $(pkgdatadir)/scripts_braille

if BUILD_MINGW32
  TEXT_KLUDGE=./sed-windows.sh
else
  TEXT_KLUDGE=./sed-linux.sh
endif


all-am:
	cd $(srcdir); ${TEXT_KLUDGE}

dist_scripts_braille_DATA = projectInfo.xml \
  cascade.xml \
  laser.xml \
  basic_lesson_01.xml \
  basic_lesson_02.xml \
  basic_lesson_03.xml \
  basic_lesson_04.xml \
  basic_lesson_05.xml \
  basic_lesson_06.xml \
  basic_lesson_07.xml \
  basic_lesson_08.xml \
  basic_lesson_09.xml \
  basic_lesson_10.xml \
  basic_lesson_11.xml \
  basic_lesson_12.xml 

EXTRA_DIST = sed-linux.rules \
  sed-linux.sh \
  sed-windows.rules \
  sed-windows.sh
