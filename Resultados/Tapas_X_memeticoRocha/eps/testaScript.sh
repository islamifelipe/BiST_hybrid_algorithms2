#!/bin/bash

#para cada par de arquivos
	#colocar os resultados do rocha e do tapas num unico arquivos
	#executar o filter do PISA (gera o conjunto referência)
	#calcular o epsilon unario aditivo para cada arquivo do par (com base no conjunto de referência calculado anteriormente)
	#juntar os dois resultados do epsilon
	#executar o teste estatistico de Mann-Whitney 
	
#esperar as concave ficarem prontas pra testar
	
for p in 50 100 200 300 400 500 600 700 800 900 #1000
do
	for i in 1 2 3 4 5
	do
		cat ../../../Tapas_rocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../Tapas_rocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log reference.out correlated/$p.corr$i/resultadoEpsilonTapas.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log reference.out correlated/$p.corr$i/resultadoEpsilonMemeticoRocha.out
		
		echo "$(cat correlated/$p.corr$i/resultadoEpsilonTapas.out)\n\n$(cat correlated/$p.corr$i/resultadoEpsilonMemeticoRocha.out)" >> correlated/$p.corr$i/uniEpsilon.out #primeiro tapas

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit correlated/$p.corr$i/uniEpsilon.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt correlated/$p.corr$i/mann_whit.out
		

		cat ../../../Tapas_rocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../Tapas_rocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log reference.out anticorrelated/$p.anticorr$i/resultadoEpsilonTapas.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log reference.out anticorrelated/$p.anticorr$i/resultadoEpsilonMemeticoRocha.out
		
		echo "$(cat anticorrelated/$p.anticorr$i/resultadoEpsilonTapas.out)\n\n$(cat anticorrelated/$p.anticorr$i/resultadoEpsilonMemeticoRocha.out)" >> anticorrelated/$p.anticorr$i/uniEpsilon.out #primeiro tapas

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit anticorrelated/$p.anticorr$i/uniEpsilon.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt anticorrelated/$p.anticorr$i/mann_whit.out
		
	done
done

