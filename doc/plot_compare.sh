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


## $1 -- env
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


## $1 -- env
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


## $1 -- compiler
## $2 -- env 1
## $3 -- env 2
## $4 -- data file
## $5 -- plot title
## $6 -- curve labels passed as reference
compiler_unroll_plot() {
    local compiler="$1"
    local env1="$2"
    local env2="$3"
    local data_file="$4"
    local plot_title="$5"
    local -n data_curves_labels=$6
    
    local plot_data_files=( "$DATA_BASE_DIR/$env1/${compiler}/${data_file}.txt"
                            "$DATA_BASE_DIR/$env1/${compiler}_unroll/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/${compiler}/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/${compiler}_unroll/${data_file}.txt"
                           )
                           
    local out_dir="$COMPARISON_DIR/$env1-$env2/$compiler"
    mkdir -p "$out_dir"
    
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
}


## $1 -- compiler
## $2 -- env 1
## $3 -- env 2
## $4 -- curve labels passed as reference
envs_compiler_plot() {
    local compiler=$1
    local env1="$2"
    local env2="$3"
    local -n data_plot_labels=$4
    
    local plot_title="Comparison of performance of $compiler on raw array"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "array_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed linked list"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "cllist_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded linked list"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "cllist_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded linked list"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "cllist_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed std::vector"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "vector_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded std::vector"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "vector_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded std::vector"
    compiler_unroll_plot "$compiler" "$env1" "$env2" "vector_st_data_plot" "$plot_title" data_plot_labels
}


## $1 -- env 1
## $2 -- env 2
## $3 -- data file
## $4 -- plot title
## $5 -- curve labels passed as reference
envs_compare_compilers_plot() {
    local env1="$1"
    local env2="$2"
    local data_file="$3"
    local plot_title="$4"
    local -n data_curves_labels=$5
    
    local plot_data_files=( "$DATA_BASE_DIR/$env1/gcc/${data_file}.txt"
                            "$DATA_BASE_DIR/$env1/clang/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/gcc/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/clang/${data_file}.txt"
                           )
                           
    local out_dir="$COMPARISON_DIR/$env1-$env2/gcc-clang"
    mkdir -p "$out_dir"
    
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
}


## $1 -- env 1
## $2 -- env 2
## $3 -- curve labels passed as reference
envs_gcc_clang_compare() {
    local env1="$1"
    local env2="$2"
    local -n data_plot_labels=$3
    
    local compiler="gcc and clang"
    
    local plot_title="Comparison of performance of $compiler on raw array"
    envs_compare_compilers_plot "$env1" "$env2" "array_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed linked list"
    envs_compare_compilers_plot "$env1" "$env2" "cllist_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded linked list"
    envs_compare_compilers_plot "$env1" "$env2" "cllist_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded linked list"
    envs_compare_compilers_plot "$env1" "$env2" "cllist_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed std::vector"
    envs_compare_compilers_plot "$env1" "$env2" "vector_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded std::vector"
    envs_compare_compilers_plot "$env1" "$env2" "vector_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded std::vector"
    envs_compare_compilers_plot "$env1" "$env2" "vector_st_data_plot" "$plot_title" data_plot_labels
}


## $1 -- env 1
## $2 -- env 2
two_envs_compilers_compare() {
    local env1="$1"
    local env2="$2"
    
    plot_labels=("gcc 7.4.0" "gcc 7.4.0 unroll" "gcc 9.2.1" "gcc 9.2.1 unroll")
    envs_compiler_plot "gcc" "$env1" "$env2" plot_labels
    
    plot_labels=("clang 6.0.0" "clang 6.0.0 unroll" "clang 9.0.0" "clang 9.0.0 unroll")
    envs_compiler_plot "clang" "$env1" "$env2" plot_labels
    
    ##gcc-clang
    plot_labels=("gcc 7.4.0" "gcc 9.2.1" "clang 6.0.0" "clang 9.0.0")
    envs_gcc_clang_compare "$env1" "$env2" plot_labels
}


## =============================================


## comparison of compilers on single device
single_env_compilers_compare i7
single_env_compilers_compare rpi3
single_env_compilers_compare i7_vbox_1
single_env_compilers_compare i7_vbox_2


## comparison of host vs virtual box #1
two_envs_compilers_compare "i7" "i7_vbox_1"


## comparison of compilers and unrolling
two_envs_compilers_compare "i7_vbox_1" "i7_vbox_2"

