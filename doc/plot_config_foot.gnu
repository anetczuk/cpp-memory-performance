###
###
###


if (show_plot eq "1") show_plot="true"
if (show_plot eq "t") show_plot="true"
if (show_plot eq "T") show_plot="true"
if (show_plot eq "TRUE") show_plot="true"

if (show_plot eq "true") {
    ## replot to png
    set terminal png
    set output output_png
    
    replot
}
