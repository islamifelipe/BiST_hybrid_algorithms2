#!/bin/bash
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		../../lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/tempo_"$p".corr"$i".out
		../../lerTempo ../../T-PL/Testes/"$p"/correlated/"$p".corr"$i"/"$p".corr"$i".timee.log

		# echo ""

		../../lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/tempo_"$p".anticorr"$i".out
		../../lerTempo ../../T-PL/Testes/"$p"/anticorrelated/"$p".anticorr"$i"/"$p".anticorr"$i".timee.log

		# echo ""


		../../lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/tempo_"$p".conc"$i".out
		../../lerTempo ../../T-PL/Testes/"$p"/concave/"$p".conc"$i"/"$p".conc"$i".timee.log

		echo ""
		

	done

done