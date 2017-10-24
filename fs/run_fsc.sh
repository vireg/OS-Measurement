
gcc file_cache2.c -o fsc.o
rm -rf fsc_results.txt

for size in $( cat sizes.txt )
do
		rm -rf fsc
		echo $size
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
		sudo nice -n -20 ./fsc.o $size
done

