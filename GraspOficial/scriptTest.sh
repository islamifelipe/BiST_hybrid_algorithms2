#!/bin/bash
c=1
while [ $c -le 30 ]; 
do

	for p in 20 30 50
	do
		for i in 1 2 3 4 5
		do
			#ran=$(shuf -i 2000-65000 -n 1);
			ran=$(( $RANDOM % 9384983948 ));
			./grasp_oficial ../InstanciasMestrado/grasp/$p.grasp$i.in $ran $p 2 0.01 Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log Teste/grasp/$p.grasp$i/$p.grasp$i.time.log > Teste/grasp/$p.grasp$i/$p.grasp"$i"_"$c".out  
		done

	done
done