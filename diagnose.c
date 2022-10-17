#include "diagnose.h"


int * diagnose(int tested_up[], int index) {
    int * state_x = (int *) malloc(NUM_NODES * sizeof(int));

    for (int i = 0; i < NUM_NODES; i++) {
        state_x[i] = 1;
    }

    int ptr = index;
    do {    
        state_x[ptr] = 0;
        ptr = tested_up[ptr];
    } while (ptr != index || tested_up[ptr] != -1);

    return state_x;
}
