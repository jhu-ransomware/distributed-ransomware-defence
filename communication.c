#include "communication.h"

void send_array(int sock, int arr[], int arr_size) {
    int buffer[arr_size];
    for (int i = 0; i < arr_size; ++i) {
        buffer[i] = htonl(arr[i]);
    }

    if (send(sock, buffer, arr_size * sizeof(int), 0) < 0) {
        perror("Error sending tested up\n");
    }
}