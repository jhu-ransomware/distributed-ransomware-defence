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

#define IP_LENGTH 16
#define PORT 10100
#define TESTING_INTERVAL 5
#define NUM_NODES 7

char name[20];
int tested_up[NUM_NODES];
int FAULTY;

void check_status(char ips[][IP_LENGTH], int num_connections);
void receiving(int server_fd);
void *receive_thread(void *server_fd);
void send_array(int sock);

int main(int argc, char const *argv[]) {
    printf("Enter name:");
    scanf("%s", name);

    printf("Enter your fault status:");
    scanf("%d", &FAULTY);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int k = 0;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FILE* file = fopen("connections.txt", "r");
    if (file == NULL) {
      perror("Error opening connections file\n");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_NODES; ++i) {
      tested_up[i] = -1;
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

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    
    int ch;
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, &server_fd); //Creating thread to keep receiving message in real time

    int ready = 0;
    printf("Enter 1 to begin testing other nodes (hit enter twice):\n"); // TODO: Fix this needing enter twice
    while (!ready) {
        scanf("%d", &ready);
    }

    printf("\n*****At any point in time enter a new fault status (1 or 0):*****");
    printf("\n");
    time_t start = time(NULL);
    while (1) {
        FD_SET(STDIN_FILENO, &readfds);
        time_t end = time(NULL);

        if (select(1, &readfds, NULL, NULL, &timeout)) {
            scanf("%d", &FAULTY);
            printf("Updated fault status to: %i\n", FAULTY);
        }
        int curr_time = difftime(end, start);
        
        if (curr_time > TESTING_INTERVAL) {
          check_status(ips, num_connections);
          start = time(NULL);
        }
    }

    close(server_fd);

    return 0;
} 

//Sending messages to port
void check_status(char ips[][IP_LENGTH], int num_connections) {
  for (int i = 0; i < num_connections; ++i) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, *(ips + i), &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }

    // Ask for fault status
    int buffer[NUM_NODES] = {0};
    send(sock, buffer, 4, 0);
    printf("\nMessage sent\n");

    recv(sock, &buffer, sizeof(buffer), 0);
    printf("Sent array values: \n");
    for (int i = 0; i < NUM_NODES; ++i) {
        int val = ntohl(buffer[i]);
        if (i < NUM_NODES - 1) printf("%d ", val);
        else printf("%d\n", val);
    }

    close(sock);
  }
}

//Calling receiving every 2 seconds
void *receive_thread(void *server_fd) {
    int s_fd = *((int *)server_fd);
    while (1)
    {
        sleep(2);
        receiving(s_fd);
    }
}

void send_array(int sock) {
    int buffer[NUM_NODES];
    for (int i = 0; i < NUM_NODES; ++i) {
        buffer[i] = htonl(tested_up[i]);
    }

    if (send(sock, buffer, NUM_NODES * sizeof(int), 0) < 0) {
        perror("Error sending tested up\n");
    }
}

//Receiving messages on our port
void receiving(int server_fd)
{
    struct sockaddr_in address;
    int valread;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1)
    {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {

                if (i == server_fd)
                {
                    int client_socket;

                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                }
                else {
                    valread = recv(i, buffer, sizeof(buffer), 0);
                    send_array(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }

        if (k == (FD_SETSIZE * 2)) break;
    }
}
