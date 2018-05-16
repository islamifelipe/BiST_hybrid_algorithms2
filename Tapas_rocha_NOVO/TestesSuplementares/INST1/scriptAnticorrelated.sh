
for k in 50 100 200 300 400 500 600 700 800 900 1000
do
    for i in 1 2 3
    do
		sh anticorrelated/"$k".anticorr"$i"/script"$k".anticorr"$i".sh
	done
done