#!/bin/bash

for p in 50 100 200 300  
do
	for i in 1 2 3 4 5
	do
		c=1
		while [ $c -le 30 ]; 
		do
		        .././memetic < ../../InstanciasMestrado/INST1/concave/$p.conc$i.in > $p.conc$i/$p.conc"$i"_"$c".out $p.conc$i/$p.PF$i.log $p.conc$i/$p.timee$i.log
		        c=$((c+1))
		done

	done

done



p=400
i=1
c=1
while [ $c -le 30 ]; 
do
        .././memetic < ../../InstanciasMestrado/INST1/concave/$p.conc$i.in > $p.conc$i/$p.conc"$i"_"$c".out $p.conc$i/$p.PF$i.log $p.conc$i/$p.timee$i.log
        c=$((c+1))
done

