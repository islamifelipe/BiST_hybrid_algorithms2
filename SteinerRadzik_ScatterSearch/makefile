
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o Grid.o Arquivo.o srss

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp -o Aresta.o -O3 -w -Wall -mms-bitfields 
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp -o Vertice.o -O3 -w -Wall -mms-bitfields 
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp -o Grafo.o -O3 -w -Wall -mms-bitfields 
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp -o Conjunto.o -O3 -w -Wall -mms-bitfields 
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp -o kruskal.o -O3 -w -Wall -mms-bitfields 

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp -o Heap.o -O3 -w -Wall -mms-bitfields 

Grid.o : Grid.cpp Grid.h
	g++ -g -c  Grid.cpp -o Grid.o -O3 -w -Wall -mms-bitfields 

Arquivo.o : Arquivo.cpp Arquivo.h
	g++ -g -c  Arquivo.cpp -o Arquivo.o -O3 -w -Wall -mms-bitfields 


margeSort.o : margeSort.cpp margeSort.h
	g++ -g -c margeSort.cpp -o margeSort.o -O3 -w -Wall -mms-bitfields 

main.o : main.cpp
	g++ -g -c  main.cpp -o main.o -O3 -w -Wall -mms-bitfields 
		
srss : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o margeSort.o Heap.o  Grid.o Arquivo.o
	g++ -o srss Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o Grid.o Arquivo.o main.o -O3 -w -Wall -mms-bitfields 
	