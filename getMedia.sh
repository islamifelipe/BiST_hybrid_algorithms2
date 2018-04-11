
for i in 50 100 200 300 400 500 600 700 800 900 1000
do
	
    	./getMedia <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc1/"$i".conc1_avaliacaoObjetivo.log
    	echo ""
		./getMedia <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc2/"$i".conc2_avaliacaoObjetivo.log
		echo ""
		./getMedia <  Tapas_rocha_NOVO/Testes/INST1/concave/"$i".conc3/"$i".conc3_avaliacaoObjetivo.log
		echo ""
done