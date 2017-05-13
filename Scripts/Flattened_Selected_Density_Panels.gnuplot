set term post portrait enhanced color font 'Courier,10'
#set output '~/u1/Paper_Plots/Selected_Density_Panels.eps'
set output 'Selected_Density_Panels.eps'
set tmargin 0
set bmargin 0
set lmargin 0
set rmargin 0
load '../Palettes/parula.pal'
set view map
set logscale cb
unset xtics
unset ytics
unset colorbox
set multiplot layout 4,3 scale 1.0, 1.0

plot '../Plots/Disk_Density_XY.0.ascii' matrix nonuniform  with image t ''

set label 1 't = 0.00 Gy' at 8.5,20.0 front right tc rgb "white"
plot '../Plots/Disk_Density_XZ.0.ascii' matrix nonuniform  with image t ''
set label 1 "" 
plot '../Plots/Disk_Density_YZ.0.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.150.ascii' matrix nonuniform  with image t ''
set label 2 't = 1.5 Gy' at 8.5,20.0 front right  tc rgb "white"
plot '../Plots/Disk_Density_XZ.150.ascii' matrix nonuniform  with image t ''
set label 2 ""
plot '../Plots/Disk_Density_YZ.150.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.300.ascii' matrix nonuniform  with image t ''
set label 3 't = 3.0 Gy' at 8.5,20.0 front right  tc rgb "white" 
plot '../Plots/Disk_Density_XZ.300.ascii' matrix nonuniform  with image t ''
set label 3 ""
plot '../Plots/Disk_Density_YZ.300.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.450.ascii' matrix nonuniform  with image t ''
set label 4 't = 4.5 Gy' at 8.5,20.0 front right  tc rgb "white"
plot '../Plots/Disk_Density_XZ.450.ascii' matrix nonuniform  with image t ''
set label 4 ""
plot '../Plots/Disk_Density_YZ.450.ascii' matrix nonuniform  with image t ''

#set label 1 'interaural time differences' at -7,1.85 front right

unset multiplot
