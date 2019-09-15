#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


run_benchmark=1
while test $# != 0; do
    case "$1" in
	    --no-benchmark|-nb) run_benchmark=0 ;;
	    *)  ;;
    esac
    shift
done


cores_num=$(grep -c ^processor /proc/cpuinfo)

echo "Detected cores: ${cores_num}"


DATA_DIR="./data"

mkdir -p $DATA_DIR


raw_file_prefix=$DATA_DIR/raw_data_cllist_mp_core
plot_file_prefix=$DATA_DIR/plot_data_cllist_mp_core


if [ $run_benchmark -ne 0 ]; then
	echo "Generating data"
	for i in $(seq 1 $cores_num); do
	   echo "Starting benchmark $i"
	   out_file=${raw_file_prefix}_${i}.txt
	   ./benchmark_cllist > $out_file &
	   pids[${i}]=$!
	done  

	## wait for all pids
	for pid in ${pids[*]}; do
	    wait $pid
	done
fi


## processing data
echo "Extracting data"
files_string=""
for i in $(seq 1 $cores_num); do
    in_file=${raw_file_prefix}_${i}.txt
    plot_data_file=${plot_file_prefix}_$i.txt
    awk -F" " '{print $1, $9, $3 $4}' $in_file > $plot_data_file
    files_string=$plot_data_file" "$files_string
done


plot_png="$DATA_DIR/${SCRIPT_NAME}.png"


echo "Plotting"
gnuplot -p -e '
			   set title "multiprocessed linked list access times"; 
			   filenames = "'"$files_string"'"; 
			   output_png = "'$plot_png'"
			  ' plot_config_multi.gnu
