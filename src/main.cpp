#include <iostream>
#include <sstream>

#include <omp.h>
#include <string.h>
#include <sys/time.h>

#include "stringBuilder.cpp"
#include "dfa.hpp"

#define DBG_GET_FINAL_STATE 0
#define CSV_OUTPUT_MICROSEC 1


using namespace std;

int get_final_state(int state, char *actions, long int start, long int end) {

    int tmp = state;

    for (long int i = start; i < end; i++) {

        if (DBG_GET_FINAL_STATE) {
            stringstream out;
            out << "state:" << actions[i] << ";action=" << tmp << "\n";
            string s;
            out >> s;
            cout << s + "\n";
        }
        tmp = get_next_state(tmp, actions[i]);

        // Speeds up the program
        if (state == 4) return 4;
    }

    return tmp;
}

int par_get_final_state(int thread_count, char * actions, long int string_length) {

    omp_set_num_threads(thread_count);

    int *mappings[thread_count];

    long int per_thread = string_length / thread_count;

    int next_state = 0;
    int thread_id;
    long int start_index, end_index;

#pragma omp parallel private(thread_id, start_index, end_index)
    {
        thread_id = omp_get_thread_num();

        start_index = per_thread * thread_id;
        end_index = per_thread * (thread_id + 1);

        if(thread_id == thread_count - 1)
            end_index = string_length;


        if (thread_id == 0) {
            next_state = get_final_state(0, actions, start_index, end_index);
        } else {
            int something[5] = {
                    4,
                    get_final_state(1, actions, start_index, end_index),
                    get_final_state(2, actions, start_index, end_index),
                    get_final_state(3, actions, start_index, end_index),
                    4
            };
            mappings[thread_id] = something;
        }
    }

    for (int i = 1; i < thread_count; i++) {
        next_state = mappings[i][next_state];
    }

    return next_state;
}

int main(int argc, char* argv[]) {

    // Number of optimistic threads can be passed as param
    int thread_count = (argc > 1) ? atoi(argv[1]) : 1;

    // Add the regular thread
    thread_count++;

    char *s = buildString();

    long int string_length = strlen(s);


    struct timeval par_start, par_end, seq_start, seq_end;

    gettimeofday(&par_start, NULL);
    int par_state = par_get_final_state(thread_count, s, string_length);
    gettimeofday(&par_end, NULL);


    gettimeofday(&seq_start, NULL);
    int seq_state = get_final_state(0, s, 0, string_length);
    gettimeofday(&seq_end, NULL);

    if (seq_state == 3 && par_state == 3) {

        long int serialTime = ((seq_end.tv_sec * 1000000 + seq_end.tv_usec)
                - (seq_start.tv_sec * 1000000 + seq_start.tv_usec));
        long int parallelTime = ((par_end.tv_sec * 1000000 + par_end.tv_usec)
                - (par_start.tv_sec * 1000000 + par_start.tv_usec));

        if (!CSV_OUTPUT_MICROSEC) {
            cout << "Sequential : " << serialTime / 1000 << "ms\n";
            cout << "  Parallel : " << parallelTime / 1000 << "ms\n";
        } else {
            cout << thread_count << "," << serialTime << "," << parallelTime << endl;
        }
        return 0;

    } else {
        cerr << "Error: Sequential and parallel should be 3:\n";
        cerr << "Sequential state: " << seq_state << "\n";
        cerr << "  Parallel state: " << par_state << "\n";
        return -1;
    }
}