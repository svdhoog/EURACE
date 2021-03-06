#Set title:
#set title "Labour Market Model"

#Set style:
#set data style linespoints
set data style lines
set pointsize 1.0
set grid
set line 5.0

set style line 6 lc rgb "black"

#Set labels:
set xlabel "Days" 
#set ylabel ""
#set y2label ""

#Set legenda:
#set nokey
set key top left

#Set ranges:
#set xrange [1:8]
#set yrange [-1:5]
#set y2range [-1:5]

#Set a tick marks:
#set xtics (1, 4, 9, 16)
#set ytics (0, 200, 400, 600, 800, 1000)
#set xtics nomirror
#set ytics nomirror
#set y2tics nomirror


#Plotting

set terminal postscript eps color solid rounded "Helvetica" 16
#set terminal postscript eps "Helvetica" 16
set output "graph-unemployment-2-skill-region-2.eps"
plot 'data-unemployment-region.csv' using 1:14 title "unemployment-rate", 'data-unemployment-region.csv' using 1:15 title "unemployment-rate-skill-group-1", 'data-unemployment-region.csv' using 1:16 title "unemployment-rate-skill-group-2", 'data-unemployment-region.csv' using 1:17 title "unemployment-rate-skill-group-3", 'data-unemployment-region.csv' using 1:18 title "unemployment-rate-skill-group-4", 'data-unemployment-region.csv' using 1:19 title "unemployment-rate-skill-group-5"  with lines ls 6
set output

set terminal png
set output "graph-unemployment-2-skill-region-2.png"
plot 'data-unemployment-region.csv' using 1:14 title "unemployment-rate", 'data-unemployment-region.csv' using 1:15 title "unemployment-rate-skill-group-1", 'data-unemployment-region.csv' using 1:16 title "unemployment-rate-skill-group-2", 'data-unemployment-region.csv' using 1:17 title "unemployment-rate-skill-group-3", 'data-unemployment-region.csv' using 1:18 title "unemployment-rate-skill-group-4", 'data-unemployment-region.csv' using 1:19 title "unemployment-rate-skill-group-5"  with lines ls 6
set output

# Call external epstopdf to transform the eps file to pdf.
# !epstopdf --outfile=graph-unemployment-2-skill-region-2.pdf graph-unemployment-2-skill-region-2.eps 
quit 
