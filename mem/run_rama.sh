
rm mem.txt

strides=(16 32 64 128 256 512 1024)
strides=(32 128 256 512)
sizes=(1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576)
#sizes=(1024 2048 4096 8192 16384 32768 131072 524288 1048576)

for size in $( echo "${sizes[*]}" )
do
	for stride in $( echo "${strides[*]}" )
	do
		#echo "stride = $stride : size = $size"
		sudo nice -n -20 ./rama.o $stride $size
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
		#echo "**************************************************"
	done
done

