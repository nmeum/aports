/* See LICENSE for license details. */

/* Delay (in seconds) used between updates of the status text. */
static const int delay = 5;

/* Separator to use between different status function outputs. */
static const char *statsep = " | ";

/* Format to use in the curtime function for the current time. */
static const char *timefmt = "%a %d %b -- %H:%M:%S";

/* Path to power supply battery file in /sys. */
static const char *sysbat = "/sys/class/power_supply/BAT0";

/* Path to control file for current charge. */
static const char *syscur = "charge_now";

/* Path to control file for full charge. */
static const char *sysfull = "charge_full_design";

/* Sound card to use for alsa output. */
static const unsigned int sndcrd = 0;

/* Name of the control channel to use for alsa output. */
static const char* swtchname = "Master Playback Switch";
static const char* volumname = "Master Playback Volume";

/* Only use batcap function if the device has a battery. */
static size_t batcapmay(char *dest, size_t n) {
	size_t ret;
	static int hasbat = -1;

	if (hasbat == -1) {
		if (access(sysbat, F_OK)) {
			hasbat = 0;
			return 0;
		}
		hasbat = 1;
	} else if (!hasbat) {
		return 0;
	}

	ret = batcap(dest, n);
	if (ret)
		ret += separator(&dest[ret], n - ret);

	return ret;
}

/* Array of functions to use in the status bar text. */
static size_t (* const sfuncs[])(char*, size_t) = {
	batcapmay,
	alsavol,
	separator,
	loadavg,
	separator,
	curtime,
};
