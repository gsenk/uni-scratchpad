reset
set key right top





set terminal pdf size 3.50in, 3.50in
set output "graf_E.pdf"

set autoscale xy
set xlabel "t"
set ylabel "x"

set pm3d map 
set title "E field over time"
splot "Maxwell_yeeE.txt" title""

set terminal windows
replot





set terminal pdf size 3.50in, 3.50in
set output "graf_B.pdf"

set autoscale xy
set xlabel "t"
set ylabel "x"

set pm3d map 
set title "B field over time"
splot "Maxwell_yeeB.txt" title""

set terminal windows
replot
