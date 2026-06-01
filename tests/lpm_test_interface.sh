#!/bin/sh

if [ "$(whoami)" != "root" ]; then
	echo "This script must be run as root"
	exit 1
fi

opt_no=
if [ ! -z "$1" ]; then
	opt_no=$1
fi

while true; do
	if [ -z "$1" ]; then
		echo "**** options ****"
		echo "1 : Terminate"
		echo "2 : LPM force on"
		echo "3 : LPM force off"
		echo "4 : LPM auto"
		echo "5 : Quit"
		echo -n " Enter choice: "
		read opt_no
	fi

	case $opt_no in
	1)
		echo "1 : Terminate"
		dbus-send --system --dest=org.freedesktop.intel_lpmd --print-reply /org/freedesktop/intel_lpmd org.freedesktop.intel_lpmd.Terminate
		;;
	2)
		echo "2 : LPM force on"
		dbus-send --system --dest=org.freedesktop.intel_lpmd --print-reply /org/freedesktop/intel_lpmd org.freedesktop.intel_lpmd.LPM_FORCE_ON
		;;
	3)
		echo "3 : LPM force off"
		dbus-send --system --dest=org.freedesktop.intel_lpmd --print-reply /org/freedesktop/intel_lpmd org.freedesktop.intel_lpmd.LPM_FORCE_OFF
		;;
	4)
		echo "4 : LPM auto"
		dbus-send --system --dest=org.freedesktop.intel_lpmd --print-reply /org/freedesktop/intel_lpmd org.freedesktop.intel_lpmd.LPM_AUTO
		;;
	5)
		exit 0
		;;
	*)
		echo "Invalid option"
	esac
	[ ! -z "$1" ] && break
done
