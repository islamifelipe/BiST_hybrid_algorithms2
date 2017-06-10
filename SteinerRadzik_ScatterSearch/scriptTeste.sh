#!/bin/bash

c=1
while [ $c -le 30 ]; 
do
	for p in 50 100 200 300 400 500 600 700 800 900 1000
	do
		for i in 1 2 3 4 5
		do
	        ./memetic < ../InstanciasMestrado/INST1/correlated/$p.corr$i.in > Teste/INST1/correlated/$p.corr$i/$p.corr"$i"_"$c".out Teste/INST1/correlated/$p.corr$i/$p.PF$i.log Teste/INST1/correlated/$p.corr$i/$p.timee$i.log
	        #c=$((c+1))

	        ./memetic < ../InstanciasMestrado/INST1/anticorrelated/$p.anticorr$i.in > Teste/INST1/anticorrelated/$p.anticorr$i/$p.anticorr"$i"_"$c".out Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log Teste/INST1/anticorrelated/$p.anticorr$i/$p.timee$i.log
	        #c=$((c+1))

	        ./memetic < ../InstanciasMestrado/INST1/concave/$p.conc$i.in > Teste/INST1/concave/$p.conc$i/$p.conc"$i"_"$c".out Teste/INST1/concave/$p.conc$i/$p.PF$i.log Teste/INST1/concave/$p.conc$i/$p.timee$i.log
	        #c=$((c+1))

		done

	done
	c=$((c+1))
done