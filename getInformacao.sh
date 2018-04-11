#!/bin/bash


for i in 50 100 200 300 400 500 600 700 800 900 1000
do
	c=1
	while [ $c -le 30 ]; 
    do

    	./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr1/"$i".corr1_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr1/"$i".corr1_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr2/"$i".corr2_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr2/"$i".corr2_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr3/"$i".corr3_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/correlated/"$i".corr3/"$i".corr3_avaliacaoObjetivo.log
		

    	./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr1/"$i".anticorr1_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr1/"$i".anticorr1_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr2/"$i".anticorr2_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr2/"$i".anticorr2_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr3/"$i".anticorr3_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/anticorrelated/"$i".anticorr3/"$i".anticorr3_avaliacaoObjetivo.log
		

    	./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc1/"$i".conc1_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc1/"$i".conc1_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc2/"$i".conc2_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc2/"$i".conc2_avaliacaoObjetivo.log
		./getInformacao <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc3/"$i".conc3_"$c".out  >> Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc3/"$i".conc3_avaliacaoObjetivo.log
		
		c=$((c+1))
    done
done 
