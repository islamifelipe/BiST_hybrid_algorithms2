#!/bin/bash
#usado pra gerar as instâncias correlated e anticorrelated (os coeficientes estao indicados abaixo)
for i in 50 100 200 300 400 500 600 700 800 900 1000
do
	./generator $i 0.1 > INST1/correlated/$i.corr1.in

	sleep 1 #como a semete do gerador aleatrorio usa o tempo, é preciso colocar o shell pra dormir pelo menos um segundo, de modo a nao gerar o mesmo time para a semente

	./generator $i 0.3 > INST1/correlated/$i.corr2.in

	sleep 1

	./generator $i 0.5 > INST1/correlated/$i.corr3.in

	sleep 1

	./generator $i 0.7 > INST1/correlated/$i.corr4.in

	sleep 1

	./generator $i 0.9 > INST1/correlated/$i.corr5.in

	sleep 1
	./generator $i -0.1 > INST1/anticorrelated/$i.anticorr1.in

	sleep 1
	./generator $i -0.3 > INST1/anticorrelated/$i.anticorr2.in

	sleep 1
	./generator $i -0.5 > INST1/anticorrelated/$i.anticorr3.in
	sleep 1
	./generator $i -0.7 > INST1/anticorrelated/$i.anticorr4.in
	sleep 1
	./generator $i -0.9 > INST1/anticorrelated/$i.anticorr5.in

done
