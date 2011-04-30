#!/bin/bash
## Created by Markus Fisch <mf@markusfisch.de> and modified slightly by David Bruce, 2011

readonly LANG=${LANG}
readonly URL="http://translate.google.com/translate_tts?tl=${LANG:0:2}&q="
readonly STOCK="${HOME}/.`basename $0`"

which mplayer &>/dev/null || {
	echo "error: mplayer not installed"
	exit
}

which wget &>/dev/null || {
	echo "error: wget not installed"
	exit
}

[ -d "${STOCK}/${LANG}" ] || {
	mkdir -p "${STOCK}/${LANG}" || {
		echo "error: cannot create speech stock \"$STOCK\""
		exit
	}
}

TEXT="`echo $* | sed 's#\ #\+#g'`"
FILE="${STOCK}/${LANG}/${TEXT}"

[ -f "${FILE}" ] ||
	wget -q -U Mozilla -O "${FILE}" "${URL}${TEXT}"

mplayer "${FILE}" &>/dev/null

