
for k in 50 100 200 300 400 500 600 700 800 900 1000
do
    for i in 1 2 3
    do
		sh concave/"$k".conc"$i"/script"$k".conc"$i".sh
	done
done