#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

##SCRIPT_NAME=$(basename "$0")
##SCRIPT_NAME=${SCRIPT_NAME%.*}


. $SCRIPT_DIR/generate_plot.sh


## $1 -- files list
## $2 -- output file
calc_plot_average() {
    local files_list=$1
    local out_file=$2
    
    local found_files=$(ls $files_list 2> /dev/null) || true
    if [ -z "$found_files" ]; then
        ## could not find files, exiting
        return ;
    fi

    
    ## reset output file
    echo "" > $out_file
    
    ## data columns
    for in_file in ${files_list}; do
        ##echo "xxx: $in_file" 
        file_join=$(cut -d " " -f 2 "${in_file}" | paste -d " " "$out_file" -)
        echo "$file_join" > "$out_file"
    done
    
    ## calculate average
    averages=$(awk '{for(i=1;i<=NF;i++) t+=$i; print (t/(i-1)); t=0}' $out_file)
    echo "$averages" > "$out_file"
    
    ## column #1 -- list size
    cut -d " " -f 1 "$SCRIPT_DIR/measurements/i7/gcc/vector_mt_data_plot_core_1.txt" > "$out_file"
    
    ## join averages
    file_join=$(echo "$averages" | paste -d " " "$out_file" -)
    echo "$file_join" > "$out_file"
}


## $1 -- data dir
calc_data_average() {
    local data_dir=$1
    
    local files_list=$data_dir/vector_mt_data_plot_core_*
    local out_file=$data_dir/vector_mt_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_curve "$out_file" "mem access" "multithreaded std::vector average" "$data_dir/plot_vector_mt_avg.png"
    
    local files_list=$data_dir/vector_mp_data_plot_proc_*
    local out_file=$data_dir/vector_mp_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_curve "$out_file" "mem access" "multiprocessed std::vector average" "$data_dir/plot_vector_mp_avg.png"
    
    local files_list=$data_dir/cllist_mt_data_plot_core_*
    local out_file=$data_dir/cllist_mt_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_curve "$out_file" "mem access" "multithreaded linked list average" "$data_dir/plot_cllist_mt_avg.png"
    
    local files_list=$data_dir/cllist_mp_data_plot_proc_*
    local out_file=$data_dir/cllist_mp_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_curve "$out_file" "mem access" "multiprocessed linked list average" "$data_dir/plot_cllist_mp_avg.png"
}


## $1 -- environment data
calc_compiler_average() {
    local env_data="$1"
    
    echo "Calculating data for $env_data"
    
    local data_dir="$env_data/gcc"
    calc_data_average "$data_dir"
    
    local data_dir="$env_data/gcc_unroll"
    calc_data_average "$data_dir"
    
    local data_dir="$env_data/clang"
    calc_data_average "$data_dir"
    
    local data_dir="$env_data/clang_unroll"
    calc_data_average "$data_dir"
}


calc_compiler_average "$SCRIPT_DIR/measurements/i7"

calc_compiler_average "$SCRIPT_DIR/measurements/rpi3"

calc_compiler_average "$SCRIPT_DIR/measurements/i7_vbox_1"

calc_compiler_average "$SCRIPT_DIR/measurements/i7_vbox_2"

