#set terminal postscript eps "Helvetica" 30
#set output "graph-monthly-output.eps"

#set terminal png
#set output "graph-monthly-output.png"

set data style linespoints
#set pointsize 4.0
#set key below
#set grid
#set line 4.0

#set xrange [1:8]
#set yrange [0:60]
#set xtics (1, 4, 9, 16)
#set ytics (0, 200, 400, 600, 800, 1000)

#set title "Labour Market Model"
set xlabel "Days"
#set ylabel "value of a"


#plot 'data-region-monthly-output.csv' using 1:2 title "monthly-output",'data-region-monthly-output.csv' using 1:3 title "monthly-sold-quantity",'data-region-monthly-output.csv' using 1:5 title "monthly-output-region-1",'data-region-monthly-output.csv' using 1:6 title "monthly-sold-quantity-region-1",'data-region-monthly-output.csv' using 1:8 title "monthly-output-region-2",'data-region-monthly-output.csv' using 1:9 title "monthly-sold-quantity-region-2"
 
plot 'data-region-monthly-output.csv' using 1:2 title "monthly-output",'data-region-monthly-output.csv' using 1:5 title "monthly-output-region-1",'data-region-monthly-output.csv' using 1:8 title "monthly-output-region-2" 
pause -1
