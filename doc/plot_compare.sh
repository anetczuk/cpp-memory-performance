#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


DATA_BASE_DIR=$SCRIPT_DIR/measurements
COMPARISON_DIR=$SCRIPT_DIR/comparison


SHOW_PLOT=0


## $1 -- curves array passed as reference
## $2 -- curve labels passed as reference
## $3 -- plot title
## $4 -- plot output file
## $5 -- use log scale for y axis
merge_curves_extended() {
    local -n curves_data=$1
        
    if [ -z "$curves_data" ]; then
        echo "No data to plot"
        return ;
    fi
    
    local -n curve_labels=$2
    local plot_title=$3
    local plot_png="$4"
    local log_scale_y="$5"

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
                    show_plot = "'"${SHOW_PLOT}"'";
                    set title "'"${plot_title}"'";                    
                    log_scale_y = "'"${log_scale_y}"'";
                    output_png = "'"${plot_png}"'";
                    call "plot_config_head.gnu";
                    '"${plot_string}"'; 
                    call "plot_config_foot.gnu";
                  '
}


## $1 -- curves array passed as reference
## $2 -- curve labels passed as reference
## $3 -- plot title
## $4 -- plot output file
merge_curves() {
    merge_curves_extended $1 $2 "$3" "$4" 0
}


## $1 -- device
## $2 -- experiment
## $3 -- title
## $4 -- use log scale for y axis
compare_compilers_plot() {
    local data_file="$2.txt"
    local plot_data_files=( "$DATA_BASE_DIR/$1/gcc/$data_file" 
                            "$DATA_BASE_DIR/$1/gcc_unroll/$data_file" 
                            "$DATA_BASE_DIR/$1/clang/$data_file" 
                            "$DATA_BASE_DIR/$1/clang_unroll/$data_file"
                            )
    local compiler_labels=("gcc" "gcc unroll" "clang" "clang unroll")
    local out_png_dir="$COMPARISON_DIR/$1"
    if [ $4 -eq 1 ]; then
        local plot_png="$out_png_dir/$2_comparison_log.png"
    else
        local plot_png="$out_png_dir/$2_comparison.png"
    fi
    
    mkdir -p "$out_png_dir"
    
    echo "$1: plotting $3"
    merge_curves_extended plot_data_files compiler_labels "$3" "$plot_png" $4
}


## $1 -- device
## $2 -- title
single_env_compilers_compare() {
    compare_compilers_plot "$1" array_st_data_plot "Comparison of single-threaded raw array access times" 0
    compare_compilers_plot "$1" vector_st_data_plot "Comparison of single-threaded std::vector access times" 0
    compare_compilers_plot "$1" cllist_st_data_plot "Comparison of single-threaded linked list access times" 0
    compare_compilers_plot "$1" cllist_st_data_plot "Comparison of single-threaded linked list access times" 1
    
    compare_compilers_plot "$1" vector_mt_data_plot_average "Comparison of multi-threaded std::vector access times" 0
    compare_compilers_plot "$1" cllist_mt_data_plot_average "Comparison of multi-threaded linked list access times" 0
    
    compare_compilers_plot "$1" vector_mp_data_plot_average "Comparison of multi-processed std::vector access times" 0
    compare_compilers_plot "$1" cllist_mp_data_plot_average "Comparison of multi-processed linked list access times" 0
}


## $1 -- env 1
## $2 -- env 2
## $3 -- compiler
## $4 -- title prefix
## $5 -- curve labels passed as reference
envs_compiler_plot() {
    local env1=$1
    local env2=$2
    local compiler=$3
    local title_prefix=$4
    ## passed as reference
    local -n data_curves_labels=$5
    
    local out_dir="$COMPARISON_DIR/$env1-$env2/$compiler"
    
    mkdir -p "$out_dir"

    local data_file="array_st_data_plot"
    local plot_title="${title_prefix} on raw array"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_st_data_plot"
    local plot_title="${title_prefix} on std::vector"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_mt_data_plot_average"
    local plot_title="${title_prefix} on multi-threaded std::vector"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_mp_data_plot_average"
    local plot_title="${title_prefix} on multi-processed std::vector"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_st_data_plot"
    local plot_title="${title_prefix} on linked list"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_mt_data_plot_average"
    local plot_title="${title_prefix} on multi-threaded linked list"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_mp_data_plot_average"
    local plot_title="${title_prefix} on multi-processed linked list"
    local plot_data_files=("$DATA_BASE_DIR/$env1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/$env2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
}


## $1 -- compiler
## $2 -- curve labels passed as reference
vbox_compiler_plot() {
    local compiler=$1
    local -n data_labels=$2
    envs_compiler_plot "i7_vbox_1" "i7_vbox_2" "$compiler" "Comparison of vbox1 and vbox2 environments" data_labels
}


## =============================================


## comparison of compilers on single device
single_env_compilers_compare i7
single_env_compilers_compare rpi3
single_env_compilers_compare i7_vbox_1
single_env_compilers_compare i7_vbox_2


## comparison of host vs virtual box #1
plot_labels=("i7" "i7 vbox #1")
envs_compiler_plot "i7" "i7_vbox_1" gcc          "Comparison of host and Virtual Box" plot_labels
envs_compiler_plot "i7" "i7_vbox_1" gcc_unroll   "Comparison of host and Virtual Box" plot_labels
envs_compiler_plot "i7" "i7_vbox_1" clang        "Comparison of host and Virtual Box" plot_labels
envs_compiler_plot "i7" "i7_vbox_1" clang_unroll "Comparison of host and Virtual Box" plot_labels


## comparison of compiler versions on vbox
plot_labels=("gcc 7.4.0" "gcc 9.2.1")
vbox_compiler_plot gcc plot_labels

plot_labels=("gcc unroll 7.4.0" "gcc unroll 9.2.1")
vbox_compiler_plot gcc_unroll plot_labels

plot_labels=("clang 7.0.0" "clang 9.0.0")
vbox_compiler_plot clang plot_labels

plot_labels=("clang unroll 7.0.0" "clang unroll 9.0.0")
vbox_compiler_plot clang_unroll plot_labels

