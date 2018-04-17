#!/bin/bash
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		../../lerPvalores   eps/correlated/"$p".corr"$i"/mann_whit.out
		# "," 
		../../lerPvalores   hypervolume/correlated/"$p".corr"$i"/mann_whit.out
		#","
		../../lerPvalores   R2/correlated/"$p".corr"$i"/mann_whit.out
		#","



		../../lerPvalores   eps/anticorrelated/"$p".anticorr"$i"/mann_whit.out
		# ","
		../../lerPvalores  hypervolume/anticorrelated/"$p".anticorr"$i"/mann_whit.out
		#","
		../../lerPvalores   R2/anticorrelated/"$p".anticorr"$i"/mann_whit.out
		#","


		
		../../lerPvalores   eps/concave/"$p".conc"$i"/mann_whit.out
		# ","
		../../lerPvalores   hypervolume/concave/"$p".conc"$i"/mann_whit.out
		#","
		../../lerPvalores   R2/concave/"$p".conc"$i"/mann_whit.out
		
		echo ""

	done

done