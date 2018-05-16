
for k in 50 100 200 300 400 500 600 700 800 900 1000
do
    for i in 1 2 3
    do
		sh correlated/"$k".corr"$i"/script"$k".corr"$i".sh
	done
done