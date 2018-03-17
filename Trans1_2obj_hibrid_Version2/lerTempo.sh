#!/bin/bash
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		../lerTempo  Testes/INST1/correlated/"$p".corr"$i"/tempo_"$p".corr"$""i".out
		# ","
		

		../lerTempo  Testes/INST1/anticorrelated/"$p".anticorr"$i"/tempo_"$p".anticorr"$""i".out
		#","
		
		../lerTempo  Testes/INST1/concave/"$p".conc"$i"/tempo_"$p".conc"$""i".out
		
		echo ""

	done

done