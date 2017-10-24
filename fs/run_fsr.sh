
gcc ran_acc.c -o fsr.o
rm -rf fsr_results.txt

for size in $( cat sizes.txt )
do
		rm -rf fsr
		echo $size
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
		sudo nice -n -20 ./fsr.o $size
done

