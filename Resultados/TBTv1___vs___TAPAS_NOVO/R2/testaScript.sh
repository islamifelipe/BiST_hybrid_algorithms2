for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 
	do
		## PRIMEIRO O TBTv1 , DEPOIS O TAPAS
		mkdir correlated/"$p".corr"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out > inputFilter.in # primeiro TBT depois o TLP
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadoR_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadoR_Tapas.out
		
		echo "$(cat correlated/"$p".corr"$i"/resultadoR_TBT.out)\n\n$(cat correlated/"$p".corr"$i"/resultadoR_Tapas.out)" >> correlated/"$p".corr"$i"/uni_R.out #primeiro TBT dopois silvia

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/"$p".corr"$i"/uni_R.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/"$p".corr"$i"/mann_whit.out
		
	done


	for i in 1 2 3 
	do
		## PRIMEIRO O TBTv1 , DEPOIS O TAPAS
		mkdir anticorrelated/"$p".anticorr"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out > inputFilter.in # primeiro TBT depois o TLP
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadoR_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadoR_Tapas.out
		
		echo "$(cat anticorrelated/"$p".anticorr"$i"/resultadoR_TBT.out)\n\n$(cat anticorrelated/"$p".anticorr"$i"/resultadoR_Tapas.out)" >> anticorrelated/"$p".anticorr"$i"/uni_R.out #primeiro TBT dopois silvia

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit anticorrelated/"$p".anticorr"$i"/uni_R.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt anticorrelated/"$p".anticorr"$i"/mann_whit.out
		
	done


	for i in 1 2 3 
	do
		## PRIMEIRO O TBTv1 , DEPOIS O TAPAS
		mkdir concave/"$p".conc"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out > inputFilter.in # primeiro TBT depois o TLP
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadoR_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/r_ind ../../../../../Epsilon/PISA/indicators_c_source/r_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadoR_Tapas.out
		
		echo "$(cat concave/"$p".conc"$i"/resultadoR_TBT.out)\n\n$(cat concave/"$p".conc"$i"/resultadoR_Tapas.out)" >> concave/"$p".conc"$i"/uni_R.out #primeiro TBT dopois silvia

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/"$p".conc"$i"/uni_R.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/"$p".conc"$i"/mann_whit.out
		
	done


done