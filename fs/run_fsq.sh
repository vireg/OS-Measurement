
gcc seq_acc.c -o fsq.o
rm -rf fsq_results.txt

for size in $( cat sizes.txt )
do
		rm -rf fsq
		echo $size
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
		sudo nice -n -20 ./fsq.o $size
done

