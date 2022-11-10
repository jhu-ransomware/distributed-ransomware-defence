/*
This just exists as a short little testing program for testing the demo UI. 
It simply sends an array to the demo program running on the same host when ran.
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int PORT = 10200;

int init_client_to_server(char ip_address[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return sock;
}

void send_msg_to_demo_node(char demo_address[], int arr[], int arr_size) {
    int sock = init_client_to_server(demo_address);
    if (sock < 0) {
        perror("Issue creating a socket\n");
        return;
    }

    int buffer_size = arr_size;
    int buffer[buffer_size];
    for (int i = 0; i < arr_size; ++i) {
        buffer[i] = htonl(arr[i]);
    }

    printf("buffer size: %d\n", buffer_size);

    for (int i = 0; i < buffer_size; ++i) {
        printf("%d\n", buffer[i]);
    }

    if (send(sock, buffer, buffer_size * sizeof(int), 0) < 0) {
        perror("Error sending demo array\n");
    }

    close(sock);
}

int main() {
    int arr[] = {0, 1, 0, 1, 0, 1, 1, 0};
    int arr_size = 8;
    char localhost[] = "127.0.0.1";
    send_msg_to_demo_node(localhost, arr, arr_size);
    return 0;
}