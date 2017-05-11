/* See LICENSE for license details. */

/* Delay (in seconds) used between updates of the status text. */
static const int delay = 5;

/* Seperator to use between different status function outputs. */
static const char *statsep = " | ";

/* Format to use in the curtime function for the current time. */
static const char *timefmt = "%d.%m.%Y -- %H:%M:%S";

/* Path to power supply battery file in /sys. */
static const char *sysbat = "/sys/class/power_supply/BAT0";

/* Sound card to use for alsa output. */
static const unsigned int sndcrd = 0;

/* Name of the control channel to use for alsa output. */
static const char* swtchname = "Master Playback Switch";
static const char* volumname = "Master Playback Volume";

/* Only use batcap function if the device has a battery. */
static size_t batcapmay(char *dest, size_t n) {
	if (access(sysbat, F_OK))
		return 0;
	else
		return batcap(dest, n);
}

/* Array of functions to use in the status bar text. */
static size_t (* const sfuncs[])(char*, size_t) = {
	batcapmay,
	seperator,
	alsavol,
	seperator,
	loadavg,
	seperator,
	curtime,
};
