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
    int this_node;

    printf("What's your node number:");
    scanf("%d", &this_node);

    printf("Enter your fault status:");
    scanf("%d", &faulty);


    FILE* file = fopen("connections.txt", "r");
    if (file == NULL) {
      perror("Error opening connections file\n");
      exit(EXIT_FAILURE);
    }

    int num_connections;
    fscanf(file, "%d ", &num_connections);

    char ips[num_connections][IP_LENGTH];
    connection nodes[num_connections];

    char line[1000]; // string to read in line from file

    // Read in list of connections
    int k = 0;
    while (fgets(line, sizeof(line), file)) {

      // Split line into tokens
      char * token = strtok(line, " ");

      // Get node number from first
      int node_num = atoi(token);
      token = strtok(NULL, " ");

      // Strip out new line and get ip addr
      token[strcspn(token, "\r\n")] = 0; // strip out new line characters
      int len = strlen(token);

      // Copy over to array struct
      strcpy(nodes[k].ip_addr, token);
      nodes[k].node_num = node_num;
      k++;
    }

    fclose(file);

    // shift nodes by appropiate amount
    int ptr = 0;
    connection connections[num_connections];
    while (nodes[ptr].node_num < this_node && ptr < num_connections) {
        ptr++;
    }
    if (ptr == num_connections) ptr = 0;

    for (int i = 0; i < num_connections; ++i) {
        connections[i] = nodes[ptr++];
        ptr = ptr % num_connections;
    }
 
    // DEBUGGING LINE
    // todo: hid behind flag
    printf("Connections (%i):\n", num_connections);
    for (int i = 0; i < num_connections; ++i) {
      printf("ip_addr:%s node_num:%i\n", connections[i].ip_addr, connections[i].node_num);
    }

    start_algo(faulty, connections, num_connections, this_node);
    return 0;
} 
