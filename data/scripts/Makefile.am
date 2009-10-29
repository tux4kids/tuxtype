## Makefile.am for tuxtype - data/scripts:
## Process with Automake to create Makefile.in

scriptsdir = $(pkgdatadir)/scripts

if BUILD_MINGW32
  TEXT_KLUDGE=./sed-windows.sh
else
  TEXT_KLUDGE=./sed-linux.sh
endif


all-am:
	cd $(srcdir); ${TEXT_KLUDGE}

dist_scripts_DATA = projectInfo.xml \
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
  basic_lesson_12.xml \
  basic_lesson_13.xml \
  basic_lesson_14.xml \
  basic_lesson_15.xml \
  basic_lesson_16.xml \
  basic_lesson_17.xml \
  basic_lesson_18.xml \
  basic_lesson_19.xml \
  basic_lesson_20.xml \
  basic_lesson_21.xml \
  basic_lesson_22.xml \
  basic_lesson_23.xml \
  basic_lesson_24.xml \
  basic_lesson_25.xml \
  basic_lesson_26.xml \
  basic_lesson_27.xml \
  basic_lesson_28.xml \
  basic_lesson_29.xml \
  basic_lesson_30.xml \
  basic_lesson_31.xml \
  basic_lesson_32.xml \
  basic_lesson_33.xml \
  basic_lesson_34.xml \
  basic_lesson_35.xml \
  basic_lesson_36.xml \
  basic_lesson_37.xml \
  basic_lesson_38.xml \
  basic_lesson_39.xml \
  basic_lesson_40.xml \
  basic_lesson_41.xml \
  basic_lesson_42.xml \
  basic_lesson_43.xml 

EXTRA_DIST = sed-linux.rules \
  sed-linux.sh \
  sed-windows.rules \
  sed-windows.sh
