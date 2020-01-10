#!/bin/bash

set -eu

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

cd $SCRIPT_DIR

SCRIPT_NAME=$(basename "$0")
SCRIPT_NAME=${SCRIPT_NAME%.*}


SHOW_PLOT=0


## $1 -- curve data file
## $2 -- curve label
## $3 -- plot title
## $4 -- plot output file
plot_data() {
    local curve_data=$1
    local curve_label=$2
    local plot_title=$3

    if [ ! -f "$curve_data" ]; then
        echo "No data to plot: $curve_data"
    else
        local plot_png="$4"
        echo "Plotting $plot_title"

        ## echo -e "plot subcommand:\n${plot_string}"
        
        gnuplot -p -e '
                        show_plot = "'"${SHOW_PLOT}"'";
                        set title "'"${plot_title}"'"; 
                        output_png = "'"${plot_png}"'";
                        call "plot_config_head.gnu";
                        plot "'"${curve_data}"'" using 1:2 title "'"${curve_label}"'" with points ls 7,
                             "'"${curve_data}"'" using 1:2 title "bezier smooth" smooth bezier ls 1 lw 2;
                        call "plot_config_foot.gnu";
                      '
    fi
}



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
    plot_data "$out_file" "mem access" "multithreaded std::vector average" "$data_dir/plot_vector_mt_avg.png"
    
    local files_list=$data_dir/vector_mp_data_plot_proc_*
    local out_file=$data_dir/vector_mp_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_data "$out_file" "mem access" "multiprocessed std::vector average" "$data_dir/plot_vector_mp_avg.png"
    
    local files_list=$data_dir/cllist_mt_data_plot_core_*
    local out_file=$data_dir/cllist_mt_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_data "$out_file" "mem access" "multithreaded linked list average" "$data_dir/plot_cllist_mt_avg.png"
    
    local files_list=$data_dir/cllist_mp_data_plot_proc_*
    local out_file=$data_dir/cllist_mp_data_plot_average.txt
    calc_plot_average "$files_list" "$out_file"
    plot_data "$out_file" "mem access" "multiprocessed linked list average" "$data_dir/plot_cllist_mp_avg.png"
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

