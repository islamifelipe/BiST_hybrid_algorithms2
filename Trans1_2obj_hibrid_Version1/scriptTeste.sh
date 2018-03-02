#!/bin/bash
cd Testes/INST1/
for k in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 
	do
		c=1
        while [ $c -le 30 ]; 
        do
			ran=$(shuf -i 2000-9999999 -n 1);
			#ran=$(( $RANDOM % 9384983948 ));
			
			./trans_"$k" $ran correlated/"$k".corr"$i"/pareto_"$k".corr$"i".out correlated/"$k".corr"$i"/tempo_"$k".corr$"i".out < ../../../InstanciasMestrado/INST1/correlated/"$k".corr"$i".in > correlated/"$k".corr"$i"/"$k".corr"$i"_"$c".out  
			c=$((c+1))
		done

		c=1
        while [ $c -le 30 ]; 
        do
			ran=$(shuf -i 2000-9999999 -n 1);
			./trans_"$k" $ran anticorrelated/"$k".anticorr"$i"/pareto_"$k".anticorr$"i".out anticorrelated/"$k".anticorr"$i"/tempo_"$k".anticorr$"i".out < ../../../InstanciasMestrado/INST1/anticorrelated/"$k".anticorr"$i".in > anticorrelated/"$k".anticorr"$i"/"$k".anticorr"$i"_"$c".out  
			c=$((c+1))
		done

		c=1
        while [ $c -le 30 ]; 
        do
			ran=$(shuf -i 2000-9999999 -n 1);
			./trans_"$k" $ran concave/"$k".conc"$i"/pareto_"$k".conc$"i".out concave/"$k".conc"$i"/tempo_"$k".conc$"i".out < ../../../InstanciasMestrado/INST1/concave/"$k".conc"$i".in > concave/"$k".conc"$i"/"$k".conc"$i"_"$c".out  
			c=$((c+1))
		done
	done

done		