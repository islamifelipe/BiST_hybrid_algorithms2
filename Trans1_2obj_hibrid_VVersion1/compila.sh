

# mkdir Testes
# mkdir Testes/INST1
# mkdir Testes/INST1/correlated
# mkdir Testes/INST1/anticorrelated
# mkdir Testes/INST1/concave
# cd Testes/INST1/
# for k in 50 100 200 300 400 500 600 700 800 900 1000
# do

# 	cd correlated
# 	mkdir "$k.corr1"
# 	mkdir "$k.corr2"
# 	mkdir "$k.corr3"
# 	cd ..
# 	cd anticorrelated
# 	mkdir "$k.anticorr1"
# 	mkdir "$k.anticorr2"
# 	mkdir "$k.anticorr3"
# 	cd ..
# 	cd concave 
# 	mkdir "$k.conc1"
# 	mkdir "$k.conc2"
# 	mkdir "$k.conc3"
# 	cd ..
	
	k=1000
	echo "#define NUMEROVERTICES $k" >> param.h
	g++ transgenetic.cpp  -Wall -O3 -o trans_"$k"
	
#done

