



#cd Testes/10_OBJETIVOS
#for k in 30 35 40 45 50 100 200 300 400 500 600 700 800 900 1000 
#do
#	mkdir $k
#	cd $k
#	mkdir k_trimmed
#	cd k_trimmed
#	mkdir correlated
#	cd correlated
#	mkdir $k.corr1
#	mkdir $k.corr2
#	mkdir $k.corr3
#	echo "" > $k.corr1/$k.corr1_sampleFile.log
#	echo "" > $k.corr2/$k.corr2_sampleFile.log
#	echo "" > $k.corr3/$k.corr3_sampleFile.log
#
#	echo "" > $k.corr1/$k.corr1_timeFile.log
#	echo "" > $k.corr2/$k.corr2_timeFile.log
#	echo "" > $k.corr3/$k.corr3_timeFile.log
#	cd ..
#	mkdir anticorrelated
#	cd anticorrelated
#	mkdir $k.anticorr1
#	mkdir $k.anticorr2
#	mkdir $k.anticorr3
#	echo "" > $k.anticorr1/$k.anticorr1_sampleFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_sampleFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_sampleFile.log
#
#	echo "" > $k.anticorr1/$k.anticorr1_timeFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_timeFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_timeFile.log
#	cd ..
#	cd .. #dentro k
#
#
#
#	mkdir k_centrum
#	cd k_centrum
#	mkdir correlated
#	cd correlated
#	mkdir $k.corr1
#	mkdir $k.corr2
#	mkdir $k.corr3
#	echo "" > $k.corr1/$k.corr1_sampleFile.log
#	echo "" > $k.corr2/$k.corr2_sampleFile.log
#	echo "" > $k.corr3/$k.corr3_sampleFile.log
#
#	echo "" > $k.corr1/$k.corr1_timeFile.log
#	echo "" > $k.corr2/$k.corr2_timeFile.log
#	echo "" > $k.corr3/$k.corr3_timeFile.log
#	cd ..
#	mkdir anticorrelated
#	cd anticorrelated
#	mkdir $k.anticorr1
#	mkdir $k.anticorr2
#	mkdir $k.anticorr3
#	echo "" > $k.anticorr1/$k.anticorr1_sampleFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_sampleFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_sampleFile.log
#
#	echo "" > $k.anticorr1/$k.anticorr1_timeFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_timeFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_timeFile.log
#	cd ..
#	cd .. #dentro k
#
#	mkdir Hurwicz
#	cd Hurwicz
#	mkdir correlated
#	cd correlated
#	mkdir $k.corr1
#	mkdir $k.corr2
#	mkdir $k.corr3
#	echo "" > $k.corr1/$k.corr1_sampleFile.log
#	echo "" > $k.corr2/$k.corr2_sampleFile.log
#	echo "" > $k.corr3/$k.corr3_sampleFile.log
#
#	echo "" > $k.corr1/$k.corr1_timeFile.log
#	echo "" > $k.corr2/$k.corr2_timeFile.log
#	echo "" > $k.corr3/$k.corr3_timeFile.log
#	cd ..
#	mkdir anticorrelated
#	cd anticorrelated
#	mkdir $k.anticorr1
#	mkdir $k.anticorr2
#	mkdir $k.anticorr3
##	echo "" > $k.anticorr1/$k.anticorr1_sampleFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_sampleFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_sampleFile.log
#
#	echo "" > $k.anticorr1/$k.anticorr1_timeFile.log
#	echo "" > $k.anticorr2/$k.anticorr2_timeFile.log
#	echo "" > $k.anticorr3/$k.anticorr3_timeFile.log
#	cd ..
#	cd .. #dentro k
#
#	cd .. #10_OBJETIVOS
#	echo "#define NUMEROVERTICES $k" >> ../../param.h
	
	 g++ transgenetic.cpp  -Wall -O3 -o trans #$15
	

#done

