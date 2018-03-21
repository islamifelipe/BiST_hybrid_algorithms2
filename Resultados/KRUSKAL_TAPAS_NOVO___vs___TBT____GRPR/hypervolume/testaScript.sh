# ordem:
# 1 TAPAS
# 2 TBT
# 3 GRPR

for p in 50 100 200 300 400 500 600 700 800 900 1000
do

	for i in 1 2 3 
	do
		mkdir correlated/"$p".corr"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../Grasp_PR/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out > inputFilter.in 
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadohyp_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Grasp_PR/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadohyp_GRASP.out
		
		echo "$(cat correlated/"$p".corr"$i"/resultadohyp_Tapas.out)\n\n$(cat correlated/"$p".corr"$i"/resultadohyp_TBT.out)\n\n$(cat correlated/"$p".corr"$i"/resultadohyp_GRASP.out)" >> correlated/"$p".corr"$i"/uni_hyp.out 

		../../../../../Epsilon/PISA/statistics_c_source/kruskal-wallis correlated/"$p".corr"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/kruskalparam.txt correlated/"$p".corr"$i"/kruskalPareado.out > correlated/"$p".corr"$i"/kruskalMulti.out
		
	done

done



for p in 50 100 200 300 400 500 600 700 800 900 1000
do

	for i in 1 2 3 
	do
		mkdir anticorrelated/"$p".anticorr"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out ../../../Grasp_PR/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out > inputFilter.in 
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadohyp_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Grasp_PR/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/"$p".anticorr"$i"/resultadohyp_GRASP.out
		
		echo "$(cat anticorrelated/"$p".anticorr"$i"/resultadohyp_Tapas.out)\n\n$(cat anticorrelated/"$p".anticorr"$i"/resultadohyp_TBT.out)\n\n$(cat anticorrelated/"$p".anticorr"$i"/resultadohyp_GRASP.out)" >> anticorrelated/"$p".anticorr"$i"/uni_hyp.out 

		../../../../../Epsilon/PISA/statistics_c_source/kruskal-wallis anticorrelated/"$p".anticorr"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/kruskalparam.txt anticorrelated/"$p".anticorr"$i"/kruskalPareado.out > anticorrelated/"$p".anticorr"$i"/kruskalMulti.out
		
	done

done


for p in 50 100 200 300 400 500 600 700 800 900 1000
do

	for i in 1 2 3 
	do
		mkdir concave/"$p".conc"$i"/
		cat ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../Grasp_PR/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out > inputFilter.in 
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha_NOVO/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadohyp_Tapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadohyp_TBT.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Grasp_PR/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/"$p".conc"$i"/resultadohyp_GRASP.out
		
		echo "$(cat concave/"$p".conc"$i"/resultadohyp_Tapas.out)\n\n$(cat concave/"$p".conc"$i"/resultadohyp_TBT.out)\n\n$(cat concave/"$p".conc"$i"/resultadohyp_GRASP.out)" >> concave/"$p".conc"$i"/uni_hyp.out 

		../../../../../Epsilon/PISA/statistics_c_source/kruskal-wallis concave/"$p".conc"$i"/uni_hyp.out ../../../../../Epsilon/PISA/statistics_c_source/kruskalparam.txt concave/"$p".conc"$i"/kruskalPareado.out > concave/"$p".conc"$i"/kruskalMulti.out
		
	done

done