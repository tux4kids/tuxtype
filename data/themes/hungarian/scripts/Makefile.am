## Makefile.am for tuxtype - data/scripts:
## Process with Automake to create Makefile.in

scriptsdir = $(pkgdatadir)/themes/hungarian/scripts

if BUILD_MINGW32
  TEXT_KLUDGE=./sed-windows.sh
else
  TEXT_KLUDGE=./sed-linux.sh
endif


all-am:
	cd $(srcdir); ${TEXT_KLUDGE}

dist_scripts_DATA = 0.xml \
  1.xml \
  2.xml \
  3.xml \
  4.xml \
  5.xml \
  6.xml \
  7.xml \
  8.xml \
  9.xml \
  aposztrof.xml \
  a.xml \
  b.xml \
  cascade.xml \
  c.xml \
  d.xml \
  egyenloseg.xml \
  e.xml \
  f.xml \
  g.xml \
  h.xml \
  i.xml \
  j.xml \
  kotojel.xml \
  k.xml \
  laser.xml \
  l.xml \
  m.xml \
  n.xml \
  o.xml \
  per.xml \
  pontosvesszo.xml \
  pont.xml \
  projectInfo.xml \
  p.xml \
  q.xml \
  r.xml \
  s.xml \
  t.xml \
  u.xml \
  vesszo.xml \
  v.xml \
  w.xml \
  x.xml \
  y.xml \
  z.xml

EXTRA_DIST = sed-linux.rules \
  sed-linux.sh \
  sed-windows.rules \
  sed-windows.sh
