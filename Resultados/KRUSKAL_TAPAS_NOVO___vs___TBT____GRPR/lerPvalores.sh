#!/bin/bash
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		./lerKruskal_first  eps/correlated/"$p".corr"$i"/kruskalMulti.out 
		# "," 
		./lerKruskal_first  hypervolume/correlated/"$p".corr"$i"/kruskalMulti.out 
		#","
		./lerKruskal_first  R2/correlated/"$p".corr"$i"/kruskalMulti.out 
		#","



		./lerKruskal_first  eps/anticorrelated/"$p".anticorr"$i"/kruskalMulti.out 
		# ","
		./lerKruskal_first  hypervolume/anticorrelated/"$p".anticorr"$i"/kruskalMulti.out 
		#","
		./lerKruskal_first  R2/anticorrelated/"$p".anticorr"$i"/kruskalMulti.out 
		#","


		
		./lerKruskal_first  eps/concave/"$p".conc"$i"/kruskalMulti.out 
		# ","
		./lerKruskal_first  hypervolume/concave/"$p".conc"$i"/kruskalMulti.out 
		#","
		./lerKruskal_first  R2/concave/"$p".conc"$i"/kruskalMulti.out 
		
		echo ""

	done

done