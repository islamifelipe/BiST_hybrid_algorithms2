#!/bin/bash

#para cada par de arquivos
	#colocar os resultados do rocha e do tapas num unico arquivos
	#executar o filter do PISA (gera o conjunto referência)
	#calcular o epsilon unario aditivo para cada arquivo do par (com base no conjunto de referência calculado anteriormente)
	#juntar os dois resultados do epsilon
	#executar o teste estatistico de Mann-Whitney 
	
#esperar as concave ficarem prontas pra testar
mkdir correlated
mkdir anticorrelated
mkdir concave

for p in 50 100 200 300 400 500 600 700 800 900 1000
do
	for i in 1 2 3 4 5
	do
		mkdir correlated/$p.corr$i
		mkdir anticorrelated/$p.anticorr$i
		mkdir concave/$p.conc$i
	done
done

