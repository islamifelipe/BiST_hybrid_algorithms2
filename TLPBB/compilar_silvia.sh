
for k in 50 #100 200 300 400 500 600 700 800 900 1000
do
	mkdir instancias/executaveis/$k
	echo "#define NUMEROVERTICES $k" >> param.h
	#g++ transgenetico.cpp rand/mersenne.o -ansi -Wall -O2 -o ../instancias/concave/executaveis/transgenetico$k
	#g++ transgenetico.cpp rand/mersenne.o -ansi -Wall -O2 -o ../instancias/correlated/executaveis/transgenetico$k
	g++ transgenetico.cpp rand/mersenne.o -Wall -w -O2 -o instancias/executaveis/$k/transv5_$k
	mkdir instancias/executaveis/$k/correlated
	mkdir instancias/executaveis/$k/anticorrelated
	mkdir instancias/executaveis/$k/concave

	for i in 1 2 3 4 5
	do
		mkdir instancias/executaveis/$k/correlated/$k.corr$i
		echo '' > instancias/executaveis/$k/correlated/$k.corr$i/$k.corr$i.timee.log
		echo '' > instancias/executaveis/$k/correlated/$k.corr$i/$k.corr$i.PF.log
		echo '' > instancias/executaveis/$k/correlated/$k.corr$i/$k.corr$i.lixo.log
		mkdir instancias/executaveis/$k/anticorrelated/$k.anticorr$i
		echo '' > instancias/executaveis/$k/anticorrelated/$k.anticorr$i/$k.anticorr$i.timee.log
		echo '' > instancias/executaveis/$k/anticorrelated/$k.anticorr$i/$k.anticorr$i.PF.log
		echo '' > instancias/executaveis/$k/anticorrelated/$k.anticorr$i/$k.anticorr$i.lixo.log
		mkdir instancias/executaveis/$k/concave/$k.conc$i
		echo '' > instancias/executaveis/$k/concave/$k.conc$i/$k.conc$i.timee.log
		echo '' > instancias/executaveis/$k/concave/$k.conc$i/$k.conc$i.PF.log
		echo '' > instancias/executaveis/$k/concave/$k.conc$i/$k.conc$i.lixo.log
		
	done
done
