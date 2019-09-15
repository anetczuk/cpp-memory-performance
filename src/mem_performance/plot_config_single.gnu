set xlabel "Memory size"
set ylabel "Time per item [ns]"
set grid

set key left top
set format x '%.0f'
set logscale x 2

set xtics ("1KB" 1024, "32KB" 32768, "256KB" 262144, "1MB" 1048576, "9MB" 9437184, "128MB" 134217728, "1GB" 1073741824)

do for [data_file in filenames] {
    plot data_file using 1:2 title "mem access" with points ls 7, \
         data_file using 1:2 title "bezier smooth" smooth bezier ls 1 lw 2
}


## replot to png
set terminal png
set output output_png
#set output 'plot.png'
replot
