
mkdir TestesSuplementares
mkdir TestesSuplementares/INST1
mkdir TestesSuplementares/INST1/correlated
mkdir TestesSuplementares/INST1/anticorrelated
mkdir TestesSuplementares/INST1/concave
cd TestesSuplementares/INST1/
for k in 50 100 200 300 400 500 600 700 800 900 1000
do

	cd correlated
	mkdir "$k.corr1"
	mkdir "$k.corr2"
	mkdir "$k.corr3"
	cd ..
	cd anticorrelated
	mkdir "$k.anticorr1"
	mkdir "$k.anticorr2"
	mkdir "$k.anticorr3"
	cd ..
	cd concave 
	mkdir "$k.conc1"
	mkdir "$k.conc2"
	mkdir "$k.conc3"
	cd ..
	
done
