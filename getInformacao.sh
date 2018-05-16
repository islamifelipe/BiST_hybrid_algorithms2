#!/bin/bash


for i in 50 100 200 300 400 500 600 700 800 900 1000
do
	c=1
	while [ $c -le 30 ]; 
    do

    	./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr1/"$i".corr1_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr1/"$i".corr1_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr2/"$i".corr2_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr2/"$i".corr2_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr3/"$i".corr3_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/correlated/"$i".corr3/"$i".corr3_sucessoBT.log 
		

		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr1/"$i".anticorr1_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr1/"$i".anticorr1_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr2/"$i".anticorr2_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr2/"$i".anticorr2_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr3/"$i".anticorr3_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/anticorrelated/"$i".anticorr3/"$i".anticorr3_sucessoBT.log 
		

		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc1/"$i".conc1_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc1/"$i".conc1_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc2/"$i".conc2_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc2/"$i".conc2_sucessoBT.log 
		./getInformacao2  Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc3/"$i".conc3_"$c".out  >> Trans1_2obj_hibrid_VVersion1/Testes/INST1/concave/"$i".conc3/"$i".conc3_sucessoBT.log 
		

		c=$((c+1))
    done
done 
