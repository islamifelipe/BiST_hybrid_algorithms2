#!/bin/bash
cd Testes/INST1/

			k=900
			i=3
			c=1
	        while [ $c -le 30 ]; 
	        do
				ran=$(( $RANDOM % 9384983948 ));
				./trans_"$k" $ran 270000 correlated/"$k".corr"$i"/pareto_"$k".corr$"i".out correlated/"$k".corr"$i"/tempo_"$k".corr$"i".out < ../../../InstanciasMestrado/INST1/correlated/"$k".corr"$i".in > correlated/"$k".corr"$i"/"$k".corr"$i"_"$c".out  
				c=$((c+1))
			done

for k in 1000 
do
	for i in 1 2 3 
	do
			c=1
	        while [ $c -le 30 ]; 
	        do
				ran=$(( $RANDOM % 9384983948 ));
				./trans_"$k" $ran 270000 correlated/"$k".corr"$i"/pareto_"$k".corr$"i".out correlated/"$k".corr"$i"/tempo_"$k".corr$"i".out < ../../../InstanciasMestrado/INST1/correlated/"$k".corr"$i".in > correlated/"$k".corr"$i"/"$k".corr"$i"_"$c".out  
				c=$((c+1))
			done
	done
done

#anti 800000
#corr 270000
#conc 270000