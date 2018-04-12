#!/bin/bash
cd Testes/INST1/
for k in 400 
do
	for i in 1 2 3 
	do
			c=1
	        while [ $c -le 30 ]; 
	        do
				ran=$(( $RANDOM % 9384983948 ));
				./trans_"$k" $ran 800000 anticorrelated/"$k".anticorr"$i"/pareto_"$k".anticorr$"i".out anticorrelated/"$k".anticorr"$i"/tempo_"$k".anticorr$"i".out < ../../../InstanciasMestrado/INST1/anticorrelated/"$k".anticorr"$i".in > anticorrelated/"$k".anticorr"$i"/"$k".anticorr"$i"_"$c".out  
				c=$((c+1))
			done
	done
done