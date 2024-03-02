/* This is a very simple SUID binary for creating an XDG_RUNTIME_DIR
 * on systems using neither PAM nor elogind. The code is basically
 * taken from dumb-runtime-dir and hence licensed under 0BSD.
 *
 * The tool creates a directory with RUNTIME_DIR_PREFIX and prints
 * its path. The caller is responsible for setting XDG_RUNTIME_DIR
 * accordingly. Contrary to the spec, the directory is never removed. */

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>

int
main(void) {
	uid_t gid = getgid();
	uid_t uid = getuid();

	/* The following has been taken from dumb-runtime-dir.
	 *
	 * See: https://github.com/ifreund/dumb_runtime_dir/blob/v1.0.4/pam_dumb_runtime_dir.c#L51-L61 */

	/* The bit size of uintmax_t will always be larger than the number of
	 * bytes needed to print it. */
	char buffer[sizeof("XDG_RUNTIME_DIR="RUNTIME_DIR_PREFIX) +
		sizeof(uintmax_t) * 8];
	/* Valid UIDs are always positive even if POSIX allows the uid_t type
	 * itself to be signed. Therefore, we can convert to uintmax_t for
	 * safe formatting. */
	int ret = snprintf(buffer, sizeof(buffer),
		"XDG_RUNTIME_DIR="RUNTIME_DIR_PREFIX"%ju", (uintmax_t)uid);
	assert(ret >= 0 && (size_t)ret < sizeof(buffer));
	const char *path = buffer + sizeof("XDG_RUNTIME_DIR=") - 1;

	if (mkdir(path, 0700) < 0) {
		/* It's ok if the directory already exists, in that case we just
		 * ensure the mode is correct before we chown(). */
		if (errno != EEXIST)
			err(EXIT_FAILURE, "mkdir failed");
		if (chmod(path, 0700) < 0)
			err(EXIT_FAILURE, "chmod failed");
	}

	if (chown(path, uid, gid) < 0)
		err(EXIT_FAILURE, "chown failed");

	printf("%s\n", path);
	return EXIT_SUCCESS;
}
