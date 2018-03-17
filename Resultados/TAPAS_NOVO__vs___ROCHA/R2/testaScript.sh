for p in 50 100 200 300 400 500 600 700 800 900 #1000
do
	for i in 1 2 3 
	do
		
		mkdir concave/"$p".conc"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/"$p".conc"$i"/"$p".PF"$i".log > inputFilter.in # primeiro TAPAS depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadoR_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/"$p".conc"$i"/"$p".PF"$i".log reference.out concave/"$p".conc"$i"/resultadoR_Rocha.out
		
		echo "$(cat concave/"$p".conc"$i"/resultadoR_Tapas.out)\n\n$(cat concave/"$p".conc"$i"/resultadoR_Rocha.out)" >> concave/"$p".conc"$i"/uni_R.out #primeiro TAPAS dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/"$p".conc"$i"/uni_R.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/"$p".conc"$i"/mann_whit.out
		
	done


done