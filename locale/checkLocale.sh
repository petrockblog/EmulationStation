#!/bin/bash

(cd lang &&
	for POLANG in *
	do
	    POFILE="./${POLANG}/LC_MESSAGES/emulationstation.po"
	    echo -n "${POLANG}: "
	    LANG=C msgfmt "${POFILE}" -o - --statistics > /dev/null
	done
 )
