#!/bin/bash

c=1
while [ $c -le 30 ]; 
do
	for p in 50 100 200 300 400 500 600 700 800 900 1000
	do
		for i in 1 2 3 4 5
		do
		    ./memetic < ../InstanciasMestrado/INST1/anticorrelated/$p.anticorr$i.in > Teste/INST1/anticorrelated/$p.anticorr$i/$p.anticorr"$i"_"$c".out Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log Teste/INST1/anticorrelated/$p.anticorr$i/$p.timee$i.log
		done

	done
	c=$((c+1))
done