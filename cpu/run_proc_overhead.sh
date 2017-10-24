max_args=7

g++ proc_overhead.c -o proc.o

for i in $( echo "0 `seq $max_args`" )
do
   echo "num_args = $i"
   sudo nice -n -20 ./proc.o $i
   echo "**************************************************"
done
