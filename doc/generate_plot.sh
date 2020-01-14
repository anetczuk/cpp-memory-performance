#!/bin/bash

set -eu


SHOW_PLOT=0


## $1 -- data files array passed as reference
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


## $1 -- curve data file
## $2 -- curve label
## $3 -- plot title
## $4 -- plot output file
plot_curve() {
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
                        log_scale_y = "0";
                        output_png = "'"${plot_png}"'";
                        call "plot_config_head.gnu";
                        plot "'"${curve_data}"'" using 1:2 title "'"${curve_label}"'" with points ls 7,
                             "'"${curve_data}"'" using 1:2 title "bezier smooth" smooth bezier ls 1 lw 2;
                        call "plot_config_foot.gnu";
                      '
    fi
}


## $1 -- data files array passed as reference
## $2 -- curve labels passed as reference
## $3 -- plot title
## $4 -- plot output file
merge_curves() {
    merge_curves_extended $1 $2 "$3" "$4" 0
}

