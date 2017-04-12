#!/bin/sh

set -e

##
# Default values
##

BASEDIR="/mnt"
ARCHES="$(uname -m)"
RELEASES="edge"
ABUILD_OPTS="-r"

##
# Functions
##

listify() {
	local list="$(printf "%s" "${@}" | tr "," " ")"
	printf "%s" "${list}"
}

foreach() {
	local a= r=
	for r in ${RELEASES}; do
		for a in ${ARCHES}; do
			n="alpinechroot-${r}-${a}"
			eval $1 "$n" "$r" "$a"
		done
	done
}

build() {
	local name="${1}" release="${2}" arch="${3}"
	sudo alpine-chroot-create -c "${BASEDIR}/${name}" \
		-a "${arch}" -r "${release}"
	sudo -E -s alpine-chroot-abuild "${BASEDIR}/${name}" ${ABUILD_OPTS}
}

destroy() {
	local name="${1}" release="${2}" arch="${3}"
	sudo -E -s alpine-chroot-destroy "${BASEDIR}/${name}"
}

##
# Go for it!
##

export APORTSDIR="$(pwd)"
keepflag=0

while getopts a:r:bk flag; do
	case "${flag}" in
		a) ARCHES="$(listify "${OPTARG}")" ;;
		b) BASEDIR="${OPTARG}" ;;
		k) keepflag=1 ;;
		r) RELEASES="$(listify "${RELEASES}")" ;;
	esac
done

shift $(expr "$OPTIND" - 1)
if [ $# -le 0 ]; then
	echo "Usage: ${0##*/} APORT..." 1>&2
	exit 1
fi

file=
for file in "${@}"; do
	[ -r "${file}/APKBUILD" ] || continue
	(cd "${file}" && foreach build)
done

[ "${keepflag}" -eq 0 ] && foreach destroy
