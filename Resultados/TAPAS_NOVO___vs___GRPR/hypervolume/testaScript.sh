for p in 50 100 200 300 400 500 600 700 #800 900 #1000
do

	for i in 1 2 3 
	do
		mkdir correlated/"$p".corr"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out > inputFilter.in # primeiro TAPAS depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadohyp_Grasp.out
		
		echo "$(cat correlated/"$p".corr"$i"/resultadohyp_Tapas.out)\n\n$(cat correlated/"$p".corr"$i"/resultadohyp_Grasp.out)" >> correlated/"$p".corr"$i"/uni_hyp.out #primeiro TAPAS dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/"$p".corr"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/"$p".corr"$i"/mann_whit.out
		
	done

	for i in 1 2 3 
	do
		#../../../../../../../Desktop/Grasp_PR/
		mkdir anticorrelated/"$p".anticorr"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out > inputFilter.in # primeiro TAPAS depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadohyp_Grasp.out
		
		echo "$(cat anticorrelated/"$p".anticorr"$i"/resultadohyp_Tapas.out)\n\n$(cat anticorrelated/"$p".anticorr"$i"/resultadohyp_Grasp.out)" >> anticorrelated/"$p".anticorr"$i"/uni_hyp.out #primeiro TAPAS dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit anticorrelated/"$p".anticorr"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt anticorrelated/"$p".anticorr"$i"/mann_whit.out
		
	done

	for i in 1 2 3 
	do
		#../../../../../../../Desktop/Grasp_PR/
		mkdir concave/"$p".conc"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out > inputFilter.in # primeiro TAPAS depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../../../../../../Desktop/Grasp_PR/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadohyp_Grasp.out
		
		echo "$(cat concave/"$p".conc"$i"/resultadohyp_Tapas.out)\n\n$(cat concave/"$p".conc"$i"/resultadohyp_Grasp.out)" >> concave/"$p".conc"$i"/uni_hyp.out #primeiro TAPAS dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/"$p".conc"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/"$p".conc"$i"/mann_whit.out
		
	done


done