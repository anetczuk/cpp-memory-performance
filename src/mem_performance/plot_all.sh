#!/bin/bash

set -eu


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


cd $SCRIPT_DIR

curr_script=$(basename $BASH_SOURCE)


argv=$@

maxmem=""
while test $# != 0; do
    case "$1" in
	    --maxmem|-maxmem) 	maxmem="$2"
	    					shift
	    					;;
	    *)  ;;
    esac
    shift
done

if [ "$maxmem" == "" ]; then
	echo "'--maxmem' argument required, exiting"
	exit 1
fi


for filename in plot_*.sh; do
	## executing script
	if [ $filename == $curr_script ]; then
		continue
	fi
	echo "Executing $filename $argv"
    ./$filename $argv
done
