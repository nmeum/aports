#!/bin/sh
# achroot can be found here <https://gist.github.com/nmeum/68d3256d0261aae1ed65>.

##
# Default values
##

BASEDIR="/mnt"
ARCHES="$(uname -m)"
RELEASES="edge"
ABUILD_OPTS="-r"
ACHROOT="$(command -v achroot 2>&1)"

##
# Functions
##

listify() {
	local list="$(printf "%s" "${@}" | tr "," " ")"
	printf "%s" "${list}"
}

foreach() {
	local a= r=
	for r in "${RELEASES}"; do
		for a in "${ARCHES}"; do
			n="alpinechroot-${r}-${a}"
			eval $1 "$n" "$r" "$a"
		done
	done
}

build() {
	local name="${1}" release="${2}" arch="${3}"
	sudo -E -s "${ACHROOT}" -d "${BASEDIR}/${name}" -k \
		-a "${arch}" -r "${release}" -- ${ABUILD_OPTS}
}

destroy() {
	local name="${1}" release="${2}" arch="${3}"
	sudo -E -s "${ACHROOT}" -d "${BASEDIR}/${name}" -b
}

##
# Go for it!
##

if [ ! -x "${ACHROOT}" ]; then
	echo "Missing achroot, please install it" 1>&2
	exit 1
fi

local keepflag=0
while getopts a:k:r:b flag; do
	case "${flag}" in
		a) ARCHES="$(listify "${OPTARG}")" ;;
		b) BASEDIR="${OPTARG}" ;;
		k) keepflag=1 ;;
		r) RELEASES="$(listify "${RELEASES}")" ;;
	esac
done

shift $((OPTIND - 1))
if [ $# -le 0 ]; then
	echo "Usage: ${0##*/} APORT..." 1>&2
	exit 1
fi

local file=
for file in "${@}"; do
	[ -r "${file}/APKBUILD" ] || continue
	(cd "${file}" && foreach build)
done

[ "${keepflag}" -eq 0 ] && foreach destroy
