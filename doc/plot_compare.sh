#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


plot_data_files="$SCRIPT_DIR/i7/gcc/vector_st_data_plot.txt $SCRIPT_DIR/i7/clang/vector_st_data_plot.txt"


## $1 -- curves array passed as reference
## $2 -- curve labels passed as reference
## $3 -- plot title
## $4 -- plot output file
merge_curves() {
    local -n curves_data=$1
    local -n curve_labels=$2
    local plot_title=$3
        
    if [ -z "$plot_data_files" ]; then
        echo "No data to plot"
    else
        local plot_png="$4"
        echo "Plotting $plot_title"
        gnuplot -p -e '
                        set title "'"$plot_title"'"; 
                        curves_num   = "'"${#curves_data[@]}"'";
                        curve_labels = "'"${curve_labels[*]}"'";
                        filenames    = "'"${curves_data[*]}"'";
                        output_png   = "'"$plot_png"'"
                      ' plot_config_multi.gnu
    fi
}


## $1 -- device
## $2 -- experiment
single_plot() {
    local plot_data_files=("$SCRIPT_DIR/$1/gcc/$2_data_plot.txt" "$SCRIPT_DIR/$1/clang/$2_data_plot.txt")
    local compiler_labels=("gcc" "clang")
    local plot_png="$1/$2_comparison.png"
    merge_curves plot_data_files compiler_labels "$3 compilers comparison" "$plot_png"
}



## comparison of compilers on single device
single_plot i7 vector_st "Intel Core i7"
single_plot rpi3 vector_st "Raspberry Pi 3"


## comparison of host vs virutal box 
plot_data_files=("$SCRIPT_DIR/i7/gcc/vector_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_1/gcc/vector_st_data_plot.txt")
plot_labels=("host" "vbox")
merge_curves plot_data_files plot_labels "Comparison of host and Virtual Box on std::vector" "i7_vbox_1/host_vector_comparison.png"

plot_data_files=("$SCRIPT_DIR/i7/gcc/cllist_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_1/gcc/cllist_st_data_plot.txt")
plot_labels=("host" "vbox")
merge_curves plot_data_files plot_labels "Comparison of host and Virtual Box on linked list" "i7_vbox_1/host_cllist_comparison.png"


## comparison of gcc versions on vbox
plot_data_files=("$SCRIPT_DIR/i7_vbox_1/gcc/vector_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/gcc/vector_st_data_plot.txt")
plot_labels=("vbox 1" "vbox 2")
merge_curves plot_data_files plot_labels "Comparison of clang versions on std::vector" "i7_vbox_2/gcc_vector_comparison.png"

plot_data_files=("$SCRIPT_DIR/i7_vbox_1/gcc/cllist_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/gcc/cllist_st_data_plot.txt")
plot_labels=("vbox 1" "vbox 2")
merge_curves plot_data_files plot_labels "Comparison of GCC versions on linked list" "i7_vbox_2/gcc_cllist_comparison.png"


## comparison of clang versions on vbox
plot_data_files=("$SCRIPT_DIR/i7_vbox_1/clang/vector_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/clang/vector_st_data_plot.txt")
plot_labels=("7.4.0" "9.2.1")
merge_curves plot_data_files plot_labels "Comparison of clang versions on std::vector" "i7_vbox_2/clang_vector_comparison.png"

plot_data_files=("$SCRIPT_DIR/i7_vbox_1/clang/cllist_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/clang/cllist_st_data_plot.txt")
plot_labels=("6.0.0" "9.0.0")
merge_curves plot_data_files plot_labels "Comparison of GCC versions on linked list" "i7_vbox_2/clang_cllist_comparison.png"


