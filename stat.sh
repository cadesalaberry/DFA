g++ -fopenmp -o dfa ./src/main.cpp

for NB_THREAD in 0 1 2 3 4
do
	./dfa $NB_THREAD >> log.log
done

rm dfa