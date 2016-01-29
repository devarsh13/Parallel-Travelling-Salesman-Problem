for i in `seq 5 1 13`
do
echo $i > input.txt
python dataGenerator.py >data.txt <input.txt

gcc TSP_serial.c -fopenmp
'./a.out' <data.txt >>serial.txt
gcc TSP_parallel.c -fopenmp -lm 
'./a.out' <data.txt >>parallel.txt
export OMP_NUM_THREADS=2
gcc TSP_parallel1.c -fopenmp -lm 
'./a.out' <data.txt >>parallel12.txt
export OMP_NUM_THREADS=4
gcc TSP_parallel1.c -fopenmp -lm 
'./a.out' <data.txt >>parallel14.txt

done
