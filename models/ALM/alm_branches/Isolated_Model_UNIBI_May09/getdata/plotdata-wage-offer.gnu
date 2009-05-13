#set terminal postscript eps "Helvetica" 30
#set output "graph-wage-offer.eps"

#set terminal png
#set output "graph-wage-offer.png"

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


plot 'data-wage-offer.csv' using 1:2 title "ave-wage-offer",'data-wage-offer.csv' using 1:3 title "ave-wage-offer-region-1",'data-wage-offer.csv' using 1:4 title "ave-wage-offer-region-2"
 

pause -1
