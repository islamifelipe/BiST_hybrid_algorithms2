for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 
	do
		mkdir concave/"$p".conc"$i"/
		cat ../../../Trans1_2obj_hibrid/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../T-PL/Testes/"$p"/concave/"$p".conc"$i"/"$p".conc"$i".PF.log > inputFilter.in # primeiro TBT depois o TLP
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../Trans1_2obj_hibrid/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/$p.conc$i/resultadoEpsTBT.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../T-PL/Testes/"$p"/concave/"$p".conc"$i"/"$p".conc"$i".PF.log reference.out concave/$p.conc$i/resultadoEpsSilvia.out
		
		echo "$(cat concave/$p.conc$i/resultadoEpsTBT.out)\n\n$(cat concave/$p.conc$i/resultadoEpsSilvia.out)" >> concave/$p.conc$i/uniEps.out #primeiro TBT dopois silvia

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/$p.conc$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/$p.conc$i/mann_whit.out
		

		
	done

done