#for k in 10 25 50 100 200 300 400 500 600 700 800 900 1000 1500
#for k in 20 30 50
for k in 18
do
	echo "#define NUMEROVERTICES $k" >> param.h
	#g++ transgenetico.cpp rand/mersenne.o -ansi -Wall -O2 -o ../instancias/concave/executaveis/transgenetico$k
	#g++ transgenetico.cpp rand/mersenne.o -ansi -Wall -O2 -o ../instancias/correlated/executaveis/transgenetico$k
	g++ transgenetico.cpp rand/mersenne.o -ansi -Wall -O2 -o ../instancias/executaveis/transv5_$k
done
