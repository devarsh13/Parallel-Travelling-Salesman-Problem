for i in `seq 5 1 13`
do
echo $i > input.txt
python dataGenerator.py >data.txt <input.txt


gcc TSP_parallel.c -fopenmp -lm 
'./a.out' <data.txt >>parallel2.txt

done
