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
        
    if [ -z "$curves_data" ]; then
        echo "No data to plot"
        return ;
    fi
    
    local plot_png="$4"

    local plot_string=""      
    plot_string="plot"
    for i in "${!curves_data[@]}"; do
        local in_file=${curves_data[$i]}
        if [ ! -f "$in_file" ]; then
            echo "Data file not found: $in_file"
            return ;
        fi 
        plot_string="${plot_string} \"$in_file\" using 1:2 title \"${curve_labels[$i]}\" with lines lw 3,"
    done

    ## echo -e "plot subcommand:\n${plot_string}"
    
    gnuplot -p -e '
                    call "plot_config_head.gnu";
                    set title "'"${plot_title}"'"; 
                    output_png = "'"${plot_png}"'";
                    '"${plot_string}"'; 
                    call "plot_config_foot.gnu";
                  '
}


## $1 -- device
## $2 -- experiment
## $3 -- title
single_plot() {
    local data_file="$2.txt"
    local plot_data_files=( "$SCRIPT_DIR/measurements/$1/gcc/$data_file" 
                            "$SCRIPT_DIR/measurements/$1/gcc_unroll/$data_file" 
                            "$SCRIPT_DIR/measurements/$1/clang/$data_file" 
                            "$SCRIPT_DIR/measurements/$1/clang_unroll/$data_file"
                            )
    local compiler_labels=("gcc" "gcc unroll" "clang" "clang unroll")
    local plot_png="measurements/$1/$2_comparison.png"
    echo "$1: plotting $3"
    merge_curves plot_data_files compiler_labels "$3" "$plot_png"
}


## $1 -- device
## $2 -- title
compiler_single_plot() {
    single_plot "$1" array_st_data_plot "Comparison of single-threaded raw array access times"
    single_plot "$1" vector_st_data_plot "Comparison of single-threaded std::vector access times"
    single_plot "$1" cllist_st_data_plot "Comparison of single-threaded linked list access times"
    
    single_plot "$1" vector_mt_data_plot_average "Comparison of multi-threaded std::vector access times"
    single_plot "$1" cllist_mt_data_plot_average "Comparison of multi-threaded linked list access times"
    
    single_plot "$1" vector_mp_data_plot_average "Comparison of multi-processed std::vector access times"
    single_plot "$1" cllist_mp_data_plot_average "Comparison of multi-processed linked list access times"
}



## comparison of compilers on single device
compiler_single_plot i7
compiler_single_plot rpi3
compiler_single_plot i7_vbox_1
compiler_single_plot i7_vbox_2

aaa() {
    ## comparison of host vs virtual box 
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
}

