set term post eps enhanced color font 'Helvetica,10'
set output "~/u1/Paper_Plots/virial_heating.eps"

plot "../Plots/Energies.dat" u 1:($4 * -2 *($1)**3.5/0.679**4) w lines lw 2,\
"../Plots/Energies.dat" u 1:5 w lines lw 2

