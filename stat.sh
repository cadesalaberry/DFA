g++ -fopenmp -o dfa ./src/main.cpp

for NB_THREAD in 1 2 3
do
	./dfa $VARIABLE >> log.log
done