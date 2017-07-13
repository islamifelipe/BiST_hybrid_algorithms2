#!/bin/bash

for k in 50 100 200 300 400 500 600 700 800 900 1000
do
	cd instancias/executaveis

	for i in 1 2 3 4 5
	do
		c=1
		while [ $c -le 30 ]; 
		do
			ran=$(( $RANDOM % 9384983948 ));
			$k/./transv5_$k ../INST1/correlated/$k.corr$i.in $ran $k/correlated/$k.corr$i/$k.corr$i.lixo.log $k/correlated/$k.corr$i/$k.corr$i.PF.log $k/correlated/$k.corr$i/$k.corr$i.timee.log > $k/correlated/$k.corr$i/$k.corr"$i"_"$c".out  
			c=$((c+1))
		done

		c=1
		while [ $c -le 30 ]; 
		do
			ran=$(( $RANDOM % 9384983948 ));
			$k/./transv5_$k ../INST1/anticorrelated/$k.anticorr$i.in $ran $k/anticorrelated/$k.anticorr$i/$k.anticorr$i.lixo.log $k/anticorrelated/$k.anticorr$i/$k.anticorr$i.PF.log $k/anticorrelated/$k.anticorr$i/$k.anticorr$i.timee.log > $k/anticorrelated/$k.anticorr$i/$k.anticorr"$i"_"$c".out  
			c=$((c+1))
		done

		c=1
		while [ $c -le 30 ]; 
		do
			ran=$(( $RANDOM % 9384983948 ));
			$k/./transv5_$k ../INST1/concave/$k.conc$i.in $ran $k/concave/$k.conc$i/$k.conc$i.lixo.log $k/concave/$k.conc$i/$k.conc$i.PF.log $k/concave/$k.conc$i/$k.conc$i.timee.log > $k/concave/$k.conc$i/$k.conc"$i"_"$c".out  
			c=$((c+1))
		done

	done
done 

