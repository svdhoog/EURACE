#Set title:
#set title "Labour Market Model"

#Set style:
#set data style linespoints
set data style lines
set pointsize 1.0
set grid
set line 5.0

#Set labels:
set xlabel "Days" 
set ylabel "wages"
#set y2label ""

#Set legenda:
#set nokey
set key top left

#Set ranges:
#set xrange [1:8]
#set yrange [1:1.3]
#set y2range [-1:5]

#Set a tick marks:
#set xtics (1, 4, 9, 16)
#set ytics (0, 200, 400, 600, 800, 1000)
#set xtics nomirror
#set ytics nomirror
#set y2tics nomirror


#Plotting

set terminal postscript eps color solid rounded "Helvetica" 16
#set terminal postscript eps monochrome "Helvetica" 16
set output 'graph-wage-2-region.eps'
plot 'data-wage-region.csv' using 1:2 title "wage",'data-wage-region.csv' using 1:8 title "wage-reg-1",'data-wage-region.csv' using 1:14 title "wage-reg-2"
set output

set terminal png
set output 'graph-wage-2-region.png'
plot 'data-wage-region.csv' using 1:2 title "wage",'data-wage-region.csv' using 1:8 title "wage-reg-1",'data-wage-region.csv' using 1:14 title "wage-reg-2"
set output

# Call external epstopdf to transform the eps file to pdf.
# !epstopdf --outfile=graph-wage-2-region.pdf graph-wage-2-region.eps 
quit 
