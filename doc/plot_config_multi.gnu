set xlabel "Memory size"
set ylabel "Time per item [ns]"
set grid

set key left top
set format x '%.0f'
set logscale x 2

set xtics ("1KB" 1024, "32KB" 32768, "256KB" 262144, "1MB" 1048576, "9MB" 9437184, "128MB" 134217728, "1GB" 1073741824)


#print "curves_num: ".curves_num
#print "filenames: ".filenames
#print "curve_labels: ".curve_labels


## words("string")


#if (x > 10000) {
#    set autoscale x
#} else {
#    set xrange [10:30]
#}


#plot for [file in filenames] file using 1:2 title "mem access" with lines lw 3

plot for [i=1:curves_num] word(filenames,i) using 1:2 title word(curve_labels,i)." mem access" with lines lw 3


## replot to png
set terminal png
set output output_png
#set output 'plot.png'
replot
