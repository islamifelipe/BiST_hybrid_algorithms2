
for k in 50 100
do	
	echo "#define NUMEROVERTICES $k" >> param.h
	g++ grasp.cpp -Wall -O3 -o grapspr_"$k" 
done