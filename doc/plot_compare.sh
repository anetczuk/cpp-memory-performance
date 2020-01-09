#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


DATA_BASE_DIR=$SCRIPT_DIR/measurements
COMPARISON_DIR=$SCRIPT_DIR/comparison


SHOW_PLOT=1


## $1 -- curves array passed as reference
## $2 -- curve labels passed as reference
## $3 -- plot title
## $4 -- plot output file
merge_curves() {
    local -n curves_data=$1
        
    if [ -z "$curves_data" ]; then
        echo "No data to plot"
        return ;
    fi
    
    local -n curve_labels=$2
    local plot_title=$3
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
                    show_plot = "'"${SHOW_PLOT}"'";
                    output_png = "'"${plot_png}"'";
                    set title "'"${plot_title}"'"; 
                    call "plot_config_head.gnu";
                    '"${plot_string}"'; 
                    call "plot_config_foot.gnu";
                  '
}


## $1 -- device
## $2 -- experiment
## $3 -- title
compare_compilers_plot() {
    local data_file="$2.txt"
    local plot_data_files=( "$DATA_BASE_DIR/$1/gcc/$data_file" 
                            "$DATA_BASE_DIR/$1/gcc_unroll/$data_file" 
                            "$DATA_BASE_DIR/$1/clang/$data_file" 
                            "$DATA_BASE_DIR/$1/clang_unroll/$data_file"
                            )
    local compiler_labels=("gcc" "gcc unroll" "clang" "clang unroll")
    local plot_png="measurements/$1/$2_comparison.png"
    echo "$1: plotting $3"
    merge_curves plot_data_files compiler_labels "$3" "$plot_png"
}


## $1 -- device
## $2 -- title
single_env_compilers_compare() {
    compare_compilers_plot "$1" array_st_data_plot "Comparison of single-threaded raw array access times"
    compare_compilers_plot "$1" vector_st_data_plot "Comparison of single-threaded std::vector access times"
    compare_compilers_plot "$1" cllist_st_data_plot "Comparison of single-threaded linked list access times"
    
    compare_compilers_plot "$1" vector_mt_data_plot_average "Comparison of multi-threaded std::vector access times"
    compare_compilers_plot "$1" cllist_mt_data_plot_average "Comparison of multi-threaded linked list access times"
    
    compare_compilers_plot "$1" vector_mp_data_plot_average "Comparison of multi-processed std::vector access times"
    compare_compilers_plot "$1" cllist_mp_data_plot_average "Comparison of multi-processed linked list access times"
}


## $1 -- env 1
## $2 -- env 2
## $3 -- compiler
## $4 -- title prefix
envs_compiler_plot() {
    local env1=$1
    local env2=$2
    local compiler=$3
    local title_prefix=$4
    
    local data_curves_labels=("$env1" "$env2")
    
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
    
    local out_dir="$COMPARISON_DIR/i7_vbox_1-i7_vbox_2/$compiler"
    
    mkdir -p "$out_dir"
    
    local data_file="array_st_data_plot"
    local plot_title="Comparison of compiler versions on raw array"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_st_data_plot"
    local plot_title="Comparison of compiler versions on std::vector"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_mt_data_plot"
    local plot_title="Comparison of compiler versions on multi-threadd std::vector"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="vector_mp_data_plot"
    local plot_title="Comparison of compiler versions on multi-processed std::vector"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_st_data_plot"
    local plot_title="Comparison of compiler versions on linked list"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_mt_data_plot"
    local plot_title="Comparison of compiler versions on multi-threadd linked list"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
    
    local data_file="cllist_mp_data_plot"
    local plot_title="Comparison of compiler versions on multi-processed linked list"
    local plot_data_files=("$DATA_BASE_DIR/i7_vbox_1/$compiler/${data_file}.txt" "$DATA_BASE_DIR/i7_vbox_2/$compiler/${data_file}.txt")
    merge_curves plot_data_files data_labels "$plot_title" "$out_dir/${data_file}.png"
}


## =============================================


## comparison of compilers on single device
SHOW_PLOT=0
disabled_for_moment01() {
single_env_compilers_compare i7
single_env_compilers_compare rpi3
single_env_compilers_compare i7_vbox_1
single_env_compilers_compare i7_vbox_2

## comparison of host vs virtual box #1
envs_compiler_plot "i7" "i7_vbox_1" gcc          "Comparison of host and Virtual Box"
envs_compiler_plot "i7" "i7_vbox_1" gcc_unroll   "Comparison of host and Virtual Box"
envs_compiler_plot "i7" "i7_vbox_1" clang        "Comparison of host and Virtual Box"
envs_compiler_plot "i7" "i7_vbox_1" clang_unroll "Comparison of host and Virtual Box"
}


dddd() {
## comparison of compiler versions on vbox
SHOW_PLOT=1
plot_labels=("gcc 7.4.0" "gcc 9.2.1")
vbox_compiler_plot gcc plot_labels

plot_labels=("gcc unroll 7.4.0" "gcc unroll 9.2.1")
vbox_compiler_plot gcc_unroll plot_labels

plot_labels=("clang 7.0.0" "clang 9.0.0")
vbox_compiler_plot clang plot_labels

plot_labels=("clang unroll 7.0.0" "clang unroll 9.0.0")
vbox_compiler_plot clang_unroll plot_labels
}


aaa() {    
    ## comparison of gcc versions on vbox
    plot_data_files=("$SCRIPT_DIR/i7_vbox_1/gcc/vector_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/gcc/vector_st_data_plot.txt")
    plot_labels=("7.4.0" "9.2.1")
    merge_curves plot_data_files plot_labels "Comparison of clang versions on std::vector" "i7_vbox_2/gcc_vector_comparison.png"
    
    plot_data_files=("$SCRIPT_DIR/i7_vbox_1/gcc/cllist_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/gcc/cllist_st_data_plot.txt")
    plot_labels=("7.4.0" "9.2.1")
    merge_curves plot_data_files plot_labels "Comparison of GCC versions on linked list" "i7_vbox_2/gcc_cllist_comparison.png"
    
    
    ## comparison of clang versions on vbox
    plot_data_files=("$SCRIPT_DIR/i7_vbox_1/clang/vector_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/clang/vector_st_data_plot.txt")
    plot_labels=("6.0.0" "9.0.0")
    merge_curves plot_data_files plot_labels "Comparison of clang versions on std::vector" "i7_vbox_2/clang_vector_comparison.png"
    
    plot_data_files=("$SCRIPT_DIR/i7_vbox_1/clang/cllist_st_data_plot.txt" "$SCRIPT_DIR/i7_vbox_2/clang/cllist_st_data_plot.txt")
    plot_labels=("6.0.0" "9.0.0")
    merge_curves plot_data_files plot_labels "Comparison of GCC versions on linked list" "i7_vbox_2/clang_cllist_comparison.png"
}

