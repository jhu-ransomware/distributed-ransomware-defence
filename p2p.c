// C program to demonstrate peer to peer chat using Socket Programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include "diagnose.h"
#include "communication.h"
#include "constants.h"
#include "adaptive.h"

int main(int argc, char const *argv[]) {
    int faulty;

    printf("Enter your fault status:");
    scanf("%d", &faulty);


    FILE* file = fopen("connections.txt", "r");
    if (file == NULL) {
      perror("Error opening connections file\n");
      exit(EXIT_FAILURE);
    }

    int num_connections;
    fscanf(file, "%d ", &num_connections);

    int connections = 0;
    char ips[num_connections][IP_LENGTH];
    char ip[IP_LENGTH];

    // Read in list of connections
    while (fgets(ip, sizeof(ip), file)) {
      ip[strcspn(ip, "\r\n")] = 0; // strip out new line characters
      strcpy(ips[connections++], ip);
    }

    fclose(file);

    // DEBUGGING LINE
    // todo: hid behind flag
    printf("Connections (%i):\n", connections);
    for (int i = 0; i < connections; ++i) {
      printf("%s\n", ips[i]);
    }

    start_algo(faulty, ips, num_connections);
    return 0;
} 
