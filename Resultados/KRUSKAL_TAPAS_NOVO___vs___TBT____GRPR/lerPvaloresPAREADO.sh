#!/bin/bash
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		./lerPvaloresPareado   eps/correlated/"$p".corr"$i"/kruskalPareado.out
		# "," 
		./lerPvaloresPareado   hypervolume/correlated/"$p".corr"$i"/kruskalPareado.out
		#","
		./lerPvaloresPareado   R2/correlated/"$p".corr"$i"/kruskalPareado.out
		#","



		./lerPvaloresPareado   eps/anticorrelated/"$p".anticorr"$i"/kruskalPareado.out
		# ","
		./lerPvaloresPareado  hypervolume/anticorrelated/"$p".anticorr"$i"/kruskalPareado.out
		#","
		./lerPvaloresPareado   R2/anticorrelated/"$p".anticorr"$i"/kruskalPareado.out
		#","


		
		./lerPvaloresPareado   eps/concave/"$p".conc"$i"/kruskalPareado.out
		# ","
		./lerPvaloresPareado   hypervolume/concave/"$p".conc"$i"/kruskalPareado.out
		#","
		./lerPvaloresPareado   R2/concave/"$p".conc"$i"/kruskalPareado.out
		
		echo ""

	done

done