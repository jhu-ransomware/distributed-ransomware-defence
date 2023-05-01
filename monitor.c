#include "monitor.h"

int run_detection(file_entr entrophies[], int arr_len) {
    int encrp_files = update_entrophy(entrophies, arr_len);
    if (encrp_files / arr_len > ENTROPHY_INCREASE_BATCH) {
        return 1;
    } else {
        return 0;
    }
}

int update_entrophy(file_entr entrophies[], int arr_len) {
    int encrp_files = 0;
    for (int i = 0; i < arr_len; ++i) {
        file_entr curr = entrophies[i];
        double entr = calc_entrophy_file(curr.filename);
        if (entr == -1) {
            encrp_files++;
        } else if ((entr - curr.entrophy) / curr.entrophy > ENTROPHY_INCREASE_FILE) {
            encrp_files++;
        }
        curr.entrophy = entr;
    }

    return encrp_files;
}