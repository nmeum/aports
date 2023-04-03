#!/bin/sh

# https://www.unicode.org/Public/emoji/15.0/emoji-test.txt
EMOJI_TEST="/usr/share/bemenu-emoji/emoji-test.txt"

# Requires WM to support EWMH standard.
active_window=$(xdotool getactivewindow)

emoji=$(awk -F '# ' '/fully-qualified/ { print $2 }' "${EMOJI_TEST}" | \
	awk -F 'E[0-9][0-9]*.[0-9][0-9]* ' '/E[0-9][0-9]*\.[0-9][0-9]*/ { print $1 $2 }' | \
	bemenu -p emoji -l 15 "$@" | cut -d ' ' -f1)

[ -n "${emoji}" ] || exit

# For some reason a high delay is needed to make this work realiably.
exec xdotool type --delay 240 --window "${active_window}" "${emoji}"
