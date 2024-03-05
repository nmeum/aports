#!/bin/sh

# https://www.unicode.org/Public/emoji/15.0/emoji-test.txt
EMOJI_TEST="/usr/share/bemenu-emoji/emoji-test.txt"

emoji=$(awk -F '# ' '/fully-qualified/ { print $2 }' "${EMOJI_TEST}" | \
	awk -F 'E[0-9][0-9]*.[0-9][0-9]* ' '/E[0-9][0-9]*\.[0-9][0-9]*/ { print $1 $2 }' | \
	bemenu -p emoji -l 15 "$@" | cut -d ' ' -f1)

[ -n "${emoji}" ] || exit
exec wtype "${emoji}"
