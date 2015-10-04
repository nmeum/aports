#!/bin/sh

##
# Default values
##

OPTS="-f /etc/lxc/default.conf"
PKGS="build-base abuild"
PREFIX="alpinebuild"
ARCHES="$(uname -m)"
RELEASES="edge"

SRCDEST="/var/cache/distfiles"
DESTDIR="$(pwd)/packages"

##
# Helper
##

listify() {
	local input="${1}"
	printf "%s\n" "${input}" | tr "," " "
}

lbind() {
	local name="${1}"
	local path="${2}"
	local dest="${3}"

	local root="$(lxc-config lxc.lxcpath)/${name}/rootfs"
	local mnt="lxc.mount.entry=${path}"

	grep -q "^${mnt} " "${root}/../config" 2>/dev/null || \
		echo "${mnt} ${dest} none bind,create=dir 0 0" >> "${root}/../config"
}

forall() {
	local cmd="${1}"
	local a= r= n=

	for a in "${ARCHES}"; do
		for r in "${RELEASES}"; do
			eval ${cmd} "${PREFIX}-$r-$a" "${r}" "${a}"
		done
	done
}

##
# Core
##

setup() {
	local name="${1}"
	local release="${2}"
	local arch="${3}"

	if ! lxc-info -n "${name}" >/dev/null 2>&1; then
		lxc-create -n "${name}" \
			-t alpine ${OPTS} -- \
			-a "${arch}" -R "${release}" build-base
		apk add --root "$(lxc-config lxc.lxcpath)/${name}/rootfs" $PKGS
	fi

	lxc-start -q -n "${name}"
	lxc-attach -n "${name}" -- /usr/sbin/adduser -s /bin/sh -D -G abuild buildozer

	lbind "${name}" "${SRCDEST}" "var/cache/distfiles"
	lbind "${name}" "${DESTDIR}" "home/buildozer/packages"
	lbind "${name}" "$(pwd)" "home/buildozer/aports"

	lxc-stop -q -n "${name}"
	lxc-start -q -n "${name}"

	lxc-attach -n "${name}" -- /bin/ln -s "/home/buildozer/aports/.abuild" "/home/buildozer/.abuild"
}

build() {
	local name="${1}"
	local release="${2}"
	local arch="${3}"

	for file in *; do
		[ -r "${file}/APKBUILD" ] || continue
		lxc-attach -n "${name}" -- su -s /bin/sh - buildozer \
			-c "cd /home/buildozer/aports/${file} && abuild -r"
	done
}

##
# Go for it!
##

if [ $(id -u) -ne 0 ]; then
	echo "${0##*/} has to be started as root." 1>&2
	exit 1
fi

while getopts -p:a:r:s: flag; do
	case "${flag}" in
		p) PREFIX="${OPTARG}" ;;
		s) SRCDEST="${OPTARG}" ;;
		a) ARCHES="$(listify "${OPTARG}")" ;;
		r) RELEASES="$(listify "${OPTARG}")" ;;
		-) break ;;
	esac
done

local pos=0 arg=
for arg in $@; do
	pos=$((pos + 1))
	if [ "${arg}" = "--" ]; then
		shift $pos
		OPTS="$@"
		break
	fi
done

mkdir -p packages
forall setup && forall build
