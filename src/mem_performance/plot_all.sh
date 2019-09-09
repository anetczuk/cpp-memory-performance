#!/bin/bash

set -eu


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"


cd $SCRIPT_DIR

curr_script=$(basename $BASH_SOURCE)


for filename in plot_*.sh; do
	## executing script
	if [ $filename == $curr_script ]; then
		continue
	fi
	echo "Executing $filename"
    ./$filename
done
