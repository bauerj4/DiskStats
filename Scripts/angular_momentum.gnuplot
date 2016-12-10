set term post eps enhanced color font 'Helvetica,10'
set output "~/u1/Paper_Plots/component_angular_momenta.eps"

plot "../Plots/HDB_Angular_Momenta.dat" u 1:(($11**2 + $12**2 + $13**2)**0.5) w lines lw 2 lc -1 lt 1,\
"../Plots/HDB_Angular_Momenta.dat" u 1:(($5**2 + $6**2 + $7**2)**0.5) w lines lw 2 lc -1 lt 1,\
"../Plots/HDB_Angular_Momenta.dat" u 1:11 w lines lw 2 lc 2 lt 2,\
"../Plots/HDB_Angular_Momenta.dat" u 1:12 w lines lw 2 lc 3 lt 2,\
"../Plots/HDB_Angular_Momenta.dat" u 1:13 w lines lw 2 lc 4 lt 2,\
"../Plots/HDB_Angular_Momenta.dat" u 1:5 w lines lw 2 lc 2 lt 3,\
"../Plots/HDB_Angular_Momenta.dat" u 1:6 w lines lw 2 lc 3 lt 3,\
"../Plots/HDB_Angular_Momenta.dat" u 1:7 w lines lw 2 lc 4 lt 3
#"../DiskStats/Plots/HDB_Angular_Momenta.dat" u 1:(($8**2 + $9**2 + $10**2)**0.5)  