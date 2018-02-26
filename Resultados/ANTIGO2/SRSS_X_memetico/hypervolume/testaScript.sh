#!/bin/bash

#para cada par de arquivos
	#colocar os resultados do rocha e do tapas num unico arquivos
	#executar o filter do PISA (gera o conjunto referência)
	#calcular o epsilon unario aditivo para cada arquivo do par (com base no conjunto de referência calculado anteriormente)
	#juntar os dois resultados do epsilon
	#executar o teste estatistico de Mann-Whitney 
	
#esperar as concave ficarem prontas pra testar

#mkdir correlated
#mkdir anticorrelated
#mkdir concave

for p in 50 100 200 300 400 500 600 700 800 900 #1000
do
	for i in 1 2 3 4 5
	do
	#	mkdir correlated/$p.corr$i
	#	mkdir anticorrelated/$p.anticorr$i
	#	mkdir concave/$p.conc$i
		

		cat ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log reference.out correlated/$p.corr$i/resultadoHypSRSS.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log reference.out correlated/$p.corr$i/resultadoHypMemeticoRocha.out
		
		echo "$(cat correlated/$p.corr$i/resultadoHypSRSS.out)\n\n$(cat correlated/$p.corr$i/resultadoHypMemeticoRocha.out)" >> correlated/$p.corr$i/uniHyp.out #primeiro SRSS

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/$p.corr$i/uniHyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/$p.corr$i/mann_whit.out
		

		cat ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log reference.out anticorrelated/$p.anticorr$i/resultadoHypSRSS.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log reference.out anticorrelated/$p.anticorr$i/resultadoHypMemeticoRocha.out
		
		echo "$(cat anticorrelated/$p.anticorr$i/resultadoHypSRSS.out)\n\n$(cat anticorrelated/$p.anticorr$i/resultadoHypMemeticoRocha.out)" >> anticorrelated/$p.anticorr$i/uniHyp.out #primeiro SRSS

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit anticorrelated/$p.anticorr$i/uniHyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt anticorrelated/$p.anticorr$i/mann_whit.out
		

		cat ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/concave/$p.conc$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../SteinerRadzik_ScatterSearch_PR/Teste/INST1/concave/$p.conc$i/$p.PF$i.log reference.out concave/$p.conc$i/resultadoHypSRSS.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log reference.out concave/$p.conc$i/resultadoHypMemeticoRocha.out
		
		echo "$(cat concave/$p.conc$i/resultadoHypSRSS.out)\n\n$(cat concave/$p.conc$i/resultadoHypMemeticoRocha.out)" >> concave/$p.conc$i/uniHyp.out #primeiro SRSS

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/$p.conc$i/uniHyp.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/$p.conc$i/mann_whit.out
		

		
	done

done

