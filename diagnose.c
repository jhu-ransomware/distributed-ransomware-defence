#include "diagnose.h"


int * diagnose(int tested_up[], int index) {
    int * state_x = (int *) malloc(NUM_NODES * sizeof(int));

    for (int i = 0; i < NUM_NODES; i++) {
        state_x[i] = 1;
    }

    int ptr = index;
    while (ptr != index) {    
        state_x[ptr] = 0;
        ptr = tested_up[ptr];
    } 

    return state_x;
}
