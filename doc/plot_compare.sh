#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


plot_data_files="$SCRIPT_DIR/i7/gcc/vector_st_data_plot.txt $SCRIPT_DIR/i7/clang/vector_st_data_plot.txt"


## $1 -- device
## $2 -- experiment
single_plot() {
    local plot_data_files=("$SCRIPT_DIR/$1/gcc/$2_data_plot.txt" "$SCRIPT_DIR/$1/clang/$2_data_plot.txt")
    local compiler_labels=("gcc" "clang")
        
    if [ -z "$plot_data_files" ]; then
    	echo "No data to plot"
    else
        plot_png="$1/$2_comparison.png"
    	echo "Plotting $1 $2"
    	gnuplot -p -e '
    				    set title "'"$3 compilers comparison"'"; 
    				    curves_num   = "'"${#plot_data_files[@]}"'";
    				    curve_labels = "'"${compiler_labels[*]}"'";
    				    filenames    = "'"${plot_data_files[*]}"'";
    				    output_png   = "'"$plot_png"'"
    				  ' plot_config_multi.gnu
    fi
}


single_plot i7 vector_st "Intel Core i7"
single_plot rpi3 vector_st "Raspberry Pi 3"
