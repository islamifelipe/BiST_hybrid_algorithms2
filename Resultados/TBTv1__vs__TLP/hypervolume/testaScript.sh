for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 
	do
		
		mkdir correlated/"$p".corr"$i"/
		cat ../../../Trans1_2obj_hibrid/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../T-PL/Testes/"$p"/correlated/"$p".corr"$i"/"$p".corr"$i".PF.log > inputFilter.in # primeiro TBT depois o TLP
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Trans1_2obj_hibrid/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/"$p".corr"$i"/resultadoHypTBT.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../T-PL/Testes/"$p"/correlated/"$p".corr"$i"/"$p".corr"$i".PF.log reference.out correlated/"$p".corr"$i"/resultadoHypSilvia.out
		
		echo "$(cat correlated/"$p".corr"$i"/resultadoHypTBT.out)\n\n$(cat correlated/"$p".corr"$i"/resultadoHypSilvia.out)" >> correlated/"$p".corr"$i"/uniHyp.out #primeiro TBT dopois silvia

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/"$p".corr"$i"/uniHyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/"$p".corr"$i"/mann_whit.out
		

		
	done

done