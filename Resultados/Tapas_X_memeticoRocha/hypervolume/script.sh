#!/bin/bash

#para cada par de arquivos
	#colocar os resultados do rocha e do tapas num unico arquivos
	#executar o filter do PISA (gera o conjunto referência)
	#calcular o hypervolume para cada arquivo do par (com base no conjunto de referência calculado anteriormente)
	#juntar os dois resultados do hypervolume
	#executar o teste estatistico de Mann-Whitney 
	
#esperar as concave ficarem prontas pra testar


for p in 50 100 200 300 400 500 600 700 800 900 #1000
do
	for i in 1 2 3 4 5
	do

		cat ../../../Tapas_rocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../Tapas_rocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log reference.out concave/$p.conc$i/resultadoHypTapas.out
		../../../../../Epsilon/PISA/indicators_c_source/hyp_ind ../../../../../Epsilon/PISA/indicators_c_source/hyp_ind_param.txt ../../../memeticRocha2006/algunsTestesRocha/Teste/INST1/concave/$p.conc$i/$p.PF$i.log reference.out concave/$p.conc$i/resultadoHypMemeticoRocha.out
		
		echo "$(cat concave/$p.conc$i/resultadoHypTapas.out)\n\n$(cat concave/$p.conc$i/resultadoHypMemeticoRocha.out)" >> concave/$p.conc$i/uniHypervolume.out #primeiro tapas

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit concave/$p.conc$i/uniHypervolume.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt concave/$p.conc$i/mann_whit.out
		
	done
done

