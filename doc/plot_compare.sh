#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

##SCRIPT_NAME=$(basename "$0")
##SCRIPT_NAME=${SCRIPT_NAME%.*}


. $SCRIPT_DIR/generate_plot.sh


DATA_BASE_DIR=$SCRIPT_DIR/measurements
COMPARISON_DIR=$SCRIPT_DIR/comparison


## $1 -- configuration 1
## $2 -- configuration 2
## $3 -- output dir
## $4 -- experiment
## $5 -- title
## $6 -- curve labels passed as reference
## $7 -- use log scale for y axis
compare_unroll_results_4() {
    local config1=$1
    local config2=$2
    local out_dir="$3"
    local data_file="$4.txt"
    local plot_title="$5"
    local -n plot_compiler_labels=$6
    
    local plot_data_files=( "$DATA_BASE_DIR/${config1}/$data_file" 
                            "$DATA_BASE_DIR/${config1}_unroll/$data_file" 
                            "$DATA_BASE_DIR/${config2}/$data_file" 
                            "$DATA_BASE_DIR/${config2}_unroll/$data_file"
                            )
                            
    if [ $7 -eq 1 ]; then
        local plot_png="$out_dir/$4_log.png"
    else
        local plot_png="$out_dir/$4.png"
    fi
    
    mkdir -p "$out_dir"
    
    echo "plotting $plot_title"
    merge_curves_extended plot_data_files plot_compiler_labels "$plot_title" "$plot_png" $7
}


## $1 -- env 1
## $2 -- env 2
## $3 -- data file
## $4 -- plot title
## $5 -- curve labels passed as reference
compare_compilers_results_4() {
    local env1="$1"
    local env2="$2"
    local data_file="$3"
    local plot_title="$4"
    local -n data_curves_labels=$5
    
    local plot_data_files=( "$DATA_BASE_DIR/$env1/gcc/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/gcc/${data_file}.txt"
                            "$DATA_BASE_DIR/$env1/clang/${data_file}.txt"
                            "$DATA_BASE_DIR/$env2/clang/${data_file}.txt"
                           )
                           
    local out_dir="$COMPARISON_DIR/$env1-$env2/gcc-clang"
    mkdir -p "$out_dir"
    
    merge_curves plot_data_files data_curves_labels "$plot_title" "$out_dir/${data_file}.png"
}


## $1 -- env
## $2 -- experiment
## $3 -- title
## $4 -- use log scale for y axis
compare_compilers_plot() {
    local env1="$1"
    
    local out_dir="$COMPARISON_DIR/$env1"
    local compiler_labels=("gcc" "gcc unroll" "clang" "clang unroll")
    
    compare_unroll_results_4 "$env1/gcc" "$env1/clang" "$out_dir" "$2" "$3" compiler_labels "$4"
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
    
    local out_dir="$COMPARISON_DIR/$env1-$env2/$compiler-${compiler}_unroll"
    
    compare_unroll_results_4 "$env1/${compiler}" "$env2/${compiler}" "$out_dir" "$data_file" "$plot_title" data_curves_labels 0
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
## $4 -- curve labels passed as reference
##
## plot order: env1 compiler, env1 compiler unroll, env2 compiler, env2 compiler unroll
##
envs_compilers_unroll_plot() {
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
## $3 -- curve labels passed as reference
##
## plot order: env1 gcc, env2 gcc, env1 clang, env2 clang
##
envs_gcc_clang_compare() {
    local env1="$1"
    local env2="$2"
    local -n data_plot_labels=$3
    
    local compiler="gcc and clang"
    
    local plot_title="Comparison of performance of $compiler on raw array"
    compare_compilers_results_4 "$env1" "$env2" "array_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed linked list"
    compare_compilers_results_4 "$env1" "$env2" "cllist_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded linked list"
    compare_compilers_results_4 "$env1" "$env2" "cllist_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded linked list"
    compare_compilers_results_4 "$env1" "$env2" "cllist_st_data_plot" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-processed std::vector"
    compare_compilers_results_4 "$env1" "$env2" "vector_mt_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on multi-threaded std::vector"
    compare_compilers_results_4 "$env1" "$env2" "vector_mp_data_plot_average" "$plot_title" data_plot_labels
    
    local plot_title="Comparison of performance of $compiler on single-threaded std::vector"
    compare_compilers_results_4 "$env1" "$env2" "vector_st_data_plot" "$plot_title" data_plot_labels
}


## $1 -- config 1
## $2 -- config 2
## $3 -- output subdir
## $4 -- curve labels passed as reference
config_compare_compilers_plot() {
    local config1="$1"
    local config2="$2"
    local out_dir="$COMPARISON_DIR/$3"
    local -n data_plot_labels=$4
    
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" array_st_data_plot "Comparison of single-threaded raw array access times" data_plot_labels 0
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" vector_st_data_plot "Comparison of single-threaded std::vector access times" data_plot_labels 0
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" cllist_st_data_plot "Comparison of single-threaded linked list access times" data_plot_labels 0
    
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" vector_mt_data_plot_average "Comparison of multi-threaded std::vector access times" data_plot_labels 0
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" cllist_mt_data_plot_average "Comparison of multi-threaded linked list access times" data_plot_labels 0
    
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" vector_mp_data_plot_average "Comparison of multi-processed std::vector access times" data_plot_labels 0
    compare_unroll_results_4 "$config1" "$config2" "$out_dir" cllist_mp_data_plot_average "Comparison of multi-processed linked list access times" data_plot_labels 0 
}



## =====================================================================================================================



#### comparison of compilers on single device
single_env_compilers_compare i7
single_env_compilers_compare rpi3
single_env_compilers_compare i7_vbox_1
single_env_compilers_compare i7_vbox_2


#### comparison of host vs virtual box #1
plot_labels=("gcc on host" "gcc on host unroll" "gcc on vbox#1" "gcc on vbox#1 unroll")
envs_compilers_unroll_plot "gcc" "i7" "i7_vbox_1" plot_labels

plot_labels=("clang on host" "clang on host unroll" "clang on vbox#1" "clang on vbox#1 unroll")
envs_compilers_unroll_plot "clang" "i7" "i7_vbox_1" plot_labels

## gcc-clang
plot_labels=("gcc on host" "gcc on vbox#1" "clang on host" "clang on vbox#1")
envs_gcc_clang_compare "i7" "i7_vbox_1" plot_labels


#### comparison of host vs virtual box #2
plot_labels=("gcc on host" "gcc on host unroll" "gcc on vbox#2" "gcc on vbox#2 unroll")
envs_compilers_unroll_plot "gcc" "i7" "i7_vbox_2" plot_labels

plot_labels=("clang on host" "clang on host unroll" "clang on vbox#2" "clang on vbox# unroll")
envs_compilers_unroll_plot "clang" "i7" "i7_vbox_2" plot_labels

## gcc-clang
plot_labels=("gcc on host" "gcc on vbox#2" "clang on host" "clang on vbox#2")
envs_gcc_clang_compare "i7" "i7_vbox_2" plot_labels


#### comparison of virtual box #1 vs virtual box #2
plot_labels=("gcc 7.4.0" "gcc 7.4.0 unroll" "gcc 9.2.1" "gcc 9.2.1 unroll")
envs_compilers_unroll_plot "gcc" "i7_vbox_1" "i7_vbox_2" plot_labels

plot_labels=("clang 6.0.0" "clang 6.0.0 unroll" "clang 9.0.0" "clang 9.0.0 unroll")
envs_compilers_unroll_plot "clang" "i7_vbox_1" "i7_vbox_2" plot_labels

## gcc-clang
plot_labels=("gcc 7.4.0" "gcc 9.2.1" "clang 6.0.0" "clang 9.0.0")
envs_gcc_clang_compare "i7_vbox_1" "i7_vbox_2" plot_labels


#### comparison of selected compilers
compiler_labels=("gcc 7.4.0" "gcc 7.4.0 unroll" "clang 9.0.0" "clang 9.0.0 unroll")
config_compare_compilers_plot "i7_vbox_1/gcc" "i7_vbox_2/clang" "gcc7-clang9" compiler_labels

compiler_labels=("gcc 9.2.1" "gcc 9.2.1 unroll" "clang 6.0.0" "clang 6.0.0 unroll")
config_compare_compilers_plot "i7_vbox_2/gcc" "i7_vbox_1/clang" "gcc9-clang7" compiler_labels
