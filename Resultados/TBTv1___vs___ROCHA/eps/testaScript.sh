for p in  50 100 200 300 400 500 600 700 800 900 1000
do

	for i in 1 2 3 
	do
		mkdir correlated/"$p".corr"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/"$p".corr"$i"/"$p".PF"$i".log > inputFilter.in # primeiro TBT depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$p".corr"$i"/pareto_"$p".corr"$i".out reference.out correlated/$p.corr$i/resultadoEpsTBT_NOVO.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/"$p".corr"$i"/"$p".PF"$i".log reference.out correlated/$p.corr$i/resultadoROCHA.out
		
		echo "$(cat correlated/$p.corr$i/resultadoEpsTBT_NOVO.out)\n\n$(cat correlated/$p.corr$i/resultadoROCHA.out)" >> correlated/$p.corr$i/uniEps.out #primeiro TBT dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/$p.corr$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/$p.corr$i/mann_whit.out
	
	done


	for i in 1 2 3 
	do
		mkdir anticorrelated/"$p".anticorr"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/"$p".anticorr"$i"/"$p".PF"$i".log > inputFilter.in # primeiro TBT depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$p".anticorr"$i"/pareto_"$p".anticorr"$i".out reference.out anticorrelated/$p.anticorr$i/resultadoEpsTBT_NOVO.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/"$p".anticorr"$i"/"$p".PF"$i".log reference.out anticorrelated/$p.anticorr$i/resultadoROCHA.out
		
		echo "$(cat anticorrelated/$p.anticorr$i/resultadoEpsTBT_NOVO.out)\n\n$(cat anticorrelated/$p.anticorr$i/resultadoROCHA.out)" >> anticorrelated/$p.anticorr$i/uniEps.out #primeiro TBT dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit anticorrelated/$p.anticorr$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt anticorrelated/$p.anticorr$i/mann_whit.out
	
	done


	for i in 1 2 3 
	do
		mkdir concave/"$p".conc"$i"/
		cat ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/"$p".conc"$i"/"$p".PF"$i".log > inputFilter.in # primeiro TBT depois o ROCHA
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$p".conc"$i"/pareto_"$p".conc"$i".out reference.out concave/$p.conc$i/resultadoEpsTBT_NOVO.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/"$p".conc"$i"/"$p".PF"$i".log reference.out concave/$p.conc$i/resultadoROCHA.out
		
		echo "$(cat concave/$p.conc$i/resultadoEpsTBT_NOVO.out)\n\n$(cat concave/$p.conc$i/resultadoROCHA.out)" >> concave/$p.conc$i/uniEps.out #primeiro TBT dopois ROCHA

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/$p.conc$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/$p.conc$i/mann_whit.out
	
	done

	

done