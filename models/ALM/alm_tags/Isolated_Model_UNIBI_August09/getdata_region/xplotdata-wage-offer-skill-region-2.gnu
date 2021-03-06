#set terminal postscript eps "Helvetica" 30
#set output "wage-offer-skill-region-2.eps"

set terminal png
set output "wage-offer-skill-region-2.png"

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


plot 'data-wage-offer-region.csv' using 1:14 title "average-wage-offer",'data-wage-offer-region.csv' using 1:15 title "wage-offer-for-skill-1",'data-wage-offer-region.csv' using 1:16 title "wage-offer-for-skill-2",'data-wage-offer-region.csv' using 1:17 title "wage-offer-for-skill-3",'data-wage-offer-region.csv' using 1:18 title "wage-offer-for-skill-4",'data-wage-offer-region.csv' using 1:19 title "wage-offer-for-skill-5"


#pause -1
