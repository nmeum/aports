#!/bin/sh
# zzz - really simple suspend script.
# Copied from void linux (licensed under public domain).

# Fixed PATH, makes it a bit saner to allow execution of this via sudoers.
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

USAGE="Usage: ${0##*/} [-nSzZR]
   -n   dry run (sleep for 5s instead of suspend/hibernate)
   -S   Low-power idle (ACPI S0)
   -z   suspend to RAM (ACPI S3) (default)
   -Z   hibernate to disk & power off (ACPI S4)
   -R   hibernate to disk & reboot"

fail() { echo ${0##*/}: 1>&2 "$*"; exit 1; }

export ZZZ_MODE=suspend
export ZZZ_HIBERNATE_MODE=platform

while getopts hnSzRZ: opt; do
  case "$opt" in
    n) ZZZ_MODE=noop ;;
    S) ZZZ_MODE=standby ;;
    z) ZZZ_MODE=suspend ;;
    Z) ZZZ_MODE=hibernate ;;
    R) ZZZ_MODE=hibernate; ZZZ_HIBERNATE_MODE=reboot ;;
    *) fail "$USAGE" ;;
  esac
done
shift $((OPTIND - 1))

case "$ZZZ_MODE" in
  suspend) grep -q mem /sys/power/state || fail "suspend not supported";;
  hibernate) grep -q disk /sys/power/state || fail "hibernate not supported";;
esac

[ -w /sys/power/state ] || fail "sleep permission denied"

(
flock -n 9 || fail "another instance of zzz is running"

case "$ZZZ_MODE" in
  standby) printf freeze >/sys/power/state || fail "standby failed";;
  suspend) printf mem >/sys/power/state || fail "suspend failed";;
  hibernate)
    echo $ZZZ_HIBERNATE_MODE >/sys/power/disk
    printf disk >/sys/power/state || fail "hibernate failed";;
  noop) sleep 5;;
esac
) 9</sys/power

# vim: et:sw=2:sts=2
