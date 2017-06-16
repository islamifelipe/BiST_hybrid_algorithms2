#!/bin/bash

for p in 20 30 50
do
	for i in 1 2 3 4 5
	do
		#verficiar se estah tudo certo com o calculo do epsilon
		cat ../../../Tapas_rocha/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log ../../../GraspOficial/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../Tapas_rocha/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log reference.out $p.grasp$i/resultadoTapasEps.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../GraspOficial/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log reference.out $p.grasp$i/resultadoArroyoEps.out

		echo "$(cat $p.grasp$i/resultadoTapasEps.out)\n\n$(cat $p.grasp$i/resultadoArroyoEps.out)" >>  $p.grasp$i/uniEps.out #primeiro tapas

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit $p.grasp$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt $p.grasp$i/mann_whit.out
		
	done
done