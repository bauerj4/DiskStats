set term post portrait enhanced color font 'Helvetica,10'
set output '~/u1/Paper_Plots/Selected_Density_Panels.eps'
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
plot '../Plots/Disk_Density_XY.250.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_XZ.250.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_YZ.250.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.275.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_XZ.275.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_YZ.275.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.350.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_XZ.350.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_YZ.350.ascii' matrix nonuniform  with image t ''

plot '../Plots/Disk_Density_XY.500.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_XZ.500.ascii' matrix nonuniform  with image t ''
plot '../Plots/Disk_Density_YZ.500.ascii' matrix nonuniform  with image t ''

unset multiplot
