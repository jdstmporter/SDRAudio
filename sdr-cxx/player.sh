#!/bin/bash


CENTRE_FREQ=${1:-433000000}
MODULATION=${2:-raw}
VOLUME=${3:-5}

unset AUDIODEV
if [ "$4" ]; then
	set AUDIODEV=$3
	echo "Outputting to ${3}"
fi

echo "Centre frequency ${CENTRE_FREQ}"
echo "Modulation ${MODULATION}"
echo "Volume gain ${VOLUME}"

rtl_fm -f ${CENTRE_FREQ} -M ${MODULATION} | play -t raw -r 24k -es -b 16 -v ${VOLUME} --ignore-length -
