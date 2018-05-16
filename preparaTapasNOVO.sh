for k in 50 100 200 300 400 500 600 700 800 900 1000
do
    for i in 1 2 3
    do
    	c=1
        while [ $c -le 30 ]; 
        do
			./getSemente $k $i $c Tapas_rocha_NOVO/Testes/INST1/concave/"$k".conc"$i"/"$k".conc"$i"_"$c".out Tapas_rocha_NOVO/TestesSuplementares/INST1/concave/"$k".conc"$i"/script"$k".conc"$i".sh
			c=$((c+1))
		done
	done
done
