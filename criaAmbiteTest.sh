#!/bin/bash

#esse script é usado para criar o ambiente de teste de algoritmo
#cada instância de cada tamanho e cada grupo tem uma pasta
#cada pasta em os arquivos de log de PF e tempo

mkdir SteinerRadzik_PR/Teste
mkdir SteinerRadzik_PR/Teste/INST1
mkdir SteinerRadzik_PR/Teste/INST1/correlated
mkdir SteinerRadzik_PR/Teste/INST1/anticorrelated
mkdir SteinerRadzik_PR/Teste/INST1/concave


for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 4 5
	do
		mkdir SteinerRadzik_PR/Teste/INST1/correlated/$p.corr$i
		echo '' > SteinerRadzik_PR/Teste/INST1/correlated/$p.corr$i/$p.PF$i.log
		echo '' > SteinerRadzik_PR/Teste/INST1/correlated/$p.corr$i/$p.timee$i.log

		mkdir SteinerRadzik_PR/Teste/INST1/anticorrelated/$p.anticorr$i
		echo '' > SteinerRadzik_PR/Teste/INST1/anticorrelated/$p.anticorr$i/$p.PF$i.log
		echo '' > SteinerRadzik_PR/Teste/INST1/anticorrelated/$p.anticorr$i/$p.timee$i.log

		mkdir SteinerRadzik_PR/Teste/INST1/concave/$p.conc$i
		echo '' > SteinerRadzik_PR/Teste/INST1/concave/$p.conc$i/$p.PF$i.log
		echo '' > SteinerRadzik_PR/Teste/INST1/concave/$p.conc$i/$p.timee$i.log

	done
done
