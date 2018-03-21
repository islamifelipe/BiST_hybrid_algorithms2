
for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3
	do
		# corr
		./lerTempo ../../Grasp_PR/Testes/INST1/correlated/"$p".corr"$i"/tempo_"$p".corr"$i".out 
		./lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/tempo_"$p".corr"$i".out
		./lerTempo ../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/tempo_"$p".corr"$i".out
	
		

		# anticorr
		./lerTempo ../../Grasp_PR/Testes/INST1/anticorrelated/"$p".anticorr"$i"/tempo_"$p".anticorr"$i".out 
		./lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/tempo_"$p".anticorr"$i".out
		./lerTempo ../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/tempo_"$p".anticorr"$i".out
	

		# conc
		./lerTempo ../../Grasp_PR/Testes/INST1/concave/"$p".conc"$i"/tempo_"$p".conc"$i".out 
		./lerTempo ../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/tempo_"$p".conc"$i".out
		./lerTempo ../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/tempo_"$p".conc"$i".out
	

		echo ""

	done
done
