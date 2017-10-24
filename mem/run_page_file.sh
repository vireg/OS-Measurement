
iterations=100

if [ -f "page_times.txt" ]; 
then 
	#echo "Removing file..."; 
	rm page_times.txt
fi

for i in $( echo "1 `seq $iterations`" )
do
	if [ -f "filex" ]; 
	then 
		#echo "Removing file..."; 
		rm filex
	fi

	./page.o
	
	sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'
done

cat page_times.txt | awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }'
