#!/bin/sh
set -e

banner() {
	printf "\n##\n# %s\n##\n\n" "${1}"
}

RDEST="${RDEST:-magnesium:/var/www/htdocs/files.8pit.net/alpine/edge/8pit}"
TDEST="pkg/8pit"

mkdir -p "${TDEST}"

banner "Retrieving packages from remote repository."
rsync -av --delete-excluded "${RDEST}/" "${TDEST}/"

banner "Invoking buildrepo."
buildrepo -a "$(pwd)" -d "$(pwd)/${TDEST%/*}" -R -p 8pit

banner "Pushing packages to remote repository."
rsync -av --delete-excluded "${TDEST}/" "${RDEST}/"
