###
###
###


set xlabel "Memory size"
set ylabel "Time per item [ns]"
set grid

set key left top
set format x '%.0f'
set logscale x 2

set xtics ("1KB" 1024, "32KB" 32768, "256KB" 262144, "1MB" 1048576, "9MB" 9437184, "128MB" 134217728, "1GB" 1073741824)


if (show_plot eq "1") show_plot="true"
if (show_plot eq "t") show_plot="true"
if (show_plot eq "T") show_plot="true"
if (show_plot eq "TRUE") show_plot="true"
if (show_plot ne "true") {
    show_plot="false"
    set terminal png
    ## print "output: ".output_png
    set output output_png
}
