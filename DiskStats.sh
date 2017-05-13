#!/bin/bash
# What is computer
#$ -S /bin/bash
#$ -cwd
#$ -M jbauer@astro.queensu.ca
#$ -o output.txt
#$ -e stderr.txt
#$ -q abaqus.q
#$ -l qname=abaqus.q
#$ -V
#$ -pe shm.pe 6
export PARALLEL=$NSLOTS
export OMP_NUM_THREADS=$NSLOTS
./bin/DiskStats++ params.tex
