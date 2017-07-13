#!/bin/bash

for p in 20 30 50
do
	for i in 1 2 3 4 5
	do
		cat ../../../SteinerRadzik_ScatterSearch/Teste/grasp/$p.grasp$i/$p.PF$i.log ../../../GraspOficial/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log > inputFilter.in
		../../../../../Epsilon/PISA/tools_c_source/filter ../../../../../Epsilon/PISA/tools_c_source/filter_param.txt inputFilter.in reference.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../SteinerRadzik_ScatterSearch/Teste/grasp/$p.grasp$i/$p.PF$i.log reference.out $p.grasp$i/resultadoSRSSEps.out
		../../../../../Epsilon/PISA/indicators_c_source/eps_ind ../../../../../Epsilon/PISA/indicators_c_source/eps_ind_param.txt ../../../GraspOficial/Teste/grasp/$p.grasp$i/$p.grasp$i.PF.log reference.out $p.grasp$i/resultadoArroyoEps.out

		echo "$(cat $p.grasp$i/resultadoSRSSEps.out)\n\n$(cat $p.grasp$i/resultadoArroyoEps.out)" >>  $p.grasp$i/uniEps.out #primeiro resultadoSRSSEps

		../../../../../Epsilon/PISA/statistics_c_source/mann-whit $p.grasp$i/uniEps.out ../../../../../Epsilon/PISA/statistics_c_source/param.txt $p.grasp$i/mann_whit.out
		
	done
done