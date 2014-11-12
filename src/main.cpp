#include <iostream>
#include <omp.h>
#include <stdio.h>
#include "stringBuilder.cpp"
#include <string.h>

using namespace std;

int getNextState(int currentState, char character) {
    if (currentState < 0 || currentState > 4) {
        if(character != 'a' && character != 'b' && character != 'c' && character != 'd') {
            printf("DONT PUT CRAP DATA INTO GETNEXTSTATE\n");
            return -1;
        }
    }
    if (currentState == 0) {
        if (character == 'a') {
            return 1;
        } else if (character == 'b') {
            return 4;
        } else if (character == 'c') {
            return 4;
        } else if (character == 'd') {
            return 4;
        }
    } else if (currentState == 1) {
        if (character == 'a') {
            return 1;
        } else if (character == 'b') {
            return 2;
        } else if (character == 'c') {
            return 4;
        } else if (character == 'd') {
            return 4;
        }
    } else if (currentState == 2) {
        if (character == 'a') {
            return 4;
        } else if (character == 'b') {
            return 2;
        } else if (character == 'c') {
            return 3;
        } else if (character == 'd') {
            return 3;
        }
    } else if (currentState == 3) {
        if (character == 'a') {
            return 1;
        } else if (character == 'b') {
            return 4;
        } else if (character == 'c') {
            return 3;
        } else if (character == 'd') {
            return 3;
        }
    } else if (currentState == 4) {
        if (character == 'a') {
            return 4;
        } else if (character == 'b') {
            return 4;
        } else if (character == 'c') {
            return 4;
        } else if (character == 'd') {
            return 4;
        }
    }
}



char* substr(char *from, int start, int end, char* to) {

    memcpy(to, &from[start], end-start);
    to[end-start] = '\0';

    return to;
}

int getFinalState(int currentState, char *string, int start, int end) {
    int state = currentState;
    int index = start;

    while (index < end) {
        state = getNextState(state, string[index]);
        index++;
    }

    return state;
}

int main() {
    int i;
    int numThreads = 4;
    char *s = buildString();
    int stringLength = (int)strlen(s);
    int stringLengthPerThread = stringLength / numThreads;
    int *mappings[numThreads];

    omp_set_num_threads(numThreads);

    for (i=0; i<numThreads; i++) {
        int something[5] = {};
        mappings[i] = something;
    }
    printf("Input: %s\n", s);

    int firstLastState = getFinalState(0, s, 0, stringLengthPerThread);
    int threadNum;
    #pragma omp parallel private(threadNum)
    {
        threadNum = omp_get_thread_num();
        int startIndex = stringLengthPerThread*threadNum;
        int endIndex = stringLengthPerThread*(threadNum+1);

        char sub[endIndex-startIndex+1];
        substr(s, startIndex, endIndex, sub);

        printf("#%d doing [%d,%d]: %s\n", threadNum, startIndex, endIndex, sub);

        mappings[threadNum][0] = getFinalState(0, s, startIndex, endIndex);
        mappings[threadNum][1] = getFinalState(1, s, startIndex, endIndex);
        mappings[threadNum][2] = getFinalState(2, s, startIndex, endIndex);
        mappings[threadNum][3] = getFinalState(3, s, startIndex, endIndex);



    }
    #pragma omp barrier

    int nextState;
    for (i=1; i<numThreads; i++) {
        nextState = mappings[i][firstLastState];
        if (nextState == 0) {
            printf("next state is 0 but it can never be that\n");
        }
        firstLastState = mappings[i][firstLastState];
    }

    //printf("Mapping[0][0]: %d\n", mappings[0][0]);
    int finalState = getFinalState(0, s, 0, stringLength);
    printf("Serial result: %d\n", finalState);
    printf("Parallel result: %d\n", firstLastState);

    return 0;
}