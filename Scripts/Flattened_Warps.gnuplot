#set term post size 2.8,1.4 landscape enhanced color font 'Courier,6'
set term post size 2.8,1.4 portrait enhanced color font 'Courier,6'
#set output '~/u1/Paper_Plots/Selected_Density_Panels.eps'
set output 'Flattened_Warps.eps'
set tmargin 0
set bmargin 0
set lmargin 0
set rmargin 0
load '../Palettes/greys.pal'
#set view map
set logscale cb
unset xtics
unset ytics
unset border
unset colorbox
set multiplot layout 2,3 #scale 1.0, 0.3

set label 1 'XZ  t = 1.75 Gy' at 23.0,20.0 front right tc rgb "black"
plot '../Plots/Disk_Density_XZ.175.ascii' matrix nonuniform  with image t ''
set label 1 ""

set label 2 'XZ  t = 1.85 Gy' at 23.0,20.0 front right tc rgb "black"
plot '../Plots/Disk_Density_XZ.185.ascii' matrix nonuniform  with image t ''
set label 2 ""

set label 3 'XZ  t = 1.95 Gy' at 23.0,20.0 front right tc rgb "black" 
plot '../Plots/Disk_Density_XZ.195.ascii' matrix nonuniform  with image t ''
set label 3 ""

set label 4 'YZ  t = 1.75 Gy' at 23.0,20.0 front right tc rgb "black"
plot '../Plots/Disk_Density_YZ.175.ascii' matrix nonuniform  with image t ''
set label 4 ""

set label 5 'YZ  t = 1.85 Gy' at 23.0,20.0 front right  tc rgb "black"
plot '../Plots/Disk_Density_YZ.185.ascii' matrix nonuniform  with image t ''
set label 5 ""

set label 6 'YZ  t = 1.95 Gy' at 23.0,20.0 front right tc rgb "black"
plot '../Plots/Disk_Density_YZ.195.ascii' matrix nonuniform  with image t ''
set label 6 ""
#set label 1 'interaural time differences' at -7,1.85 front right

unset multiplot
