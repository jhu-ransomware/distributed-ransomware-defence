#include "adaptive.h"

int tested_up[NUM_NODES];
int FAULTY;

void start_algo(int faulty, connection connections[], int num_connections, int node_num) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int k = 0;
    FAULTY = faulty;

    for (int i = 0; i < NUM_NODES; ++i) {
        tested_up[i] = -1;
    }

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
    
    int ch;
    pthread_t tid;
    pthread_create(&tid, NULL, &receive_thread, &server_fd); //Creating thread to keep receiving message in real time

    int ready = 0;
    printf("Enter 1 to begin testing other nodes:\n"); // TODO: Fix this needing enter twice
    while (!ready) {
        scanf("%d", &ready);
    }

    adaptive_dsd(faulty, connections, num_connections, node_num);

    close(server_fd);
}

void adaptive_dsd(int faulty, connection connections[], int num_connections, int node_num) {
    fd_set readfds;
    FD_ZERO(&readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    printf("\n*****At any point in time enter a new fault status (1 or 0):*****");
    printf("\n");
    time_t start = time(NULL);
    while (1) {
        FD_SET(STDIN_FILENO, &readfds);
        time_t end = time(NULL);

        if (select(1, &readfds, NULL, NULL, &timeout)) {
            int input;
            scanf("%d", &input);
            if (input == 0 || input == 1) {
                FAULTY = input;
                printf("Fault status changed to %d");
            } 
            else if (input == 2) {
                int * diagnosis = diagnose(tested_up, node_num);
                printf("Diagnosis: \n");
                for (int i = 0; i < NUM_NODES; ++i) {
                    if (diagnosis[i] == 1) {
                        printf("Node %d is faulty \n", i);
                    }
                    else {
                        printf("Node %d is not faulty \n", i);
                    }
                }
            }
            else {
                printf("Invalid input. Enter 1 or 0 to change fault status, or 2 to diagnose.");
            }
        }

        int curr_time = difftime(end, start);
        
        if (curr_time > TESTING_INTERVAL) {
          update_arr(connections, num_connections, node_num);
          start = time(NULL);
        }
    }
}

//Calling receiving every 2 seconds
void *receive_thread(void *server_fd) {
    int s_fd = *((int *)server_fd);
    while (1) {
        sleep(2);
        receiving(s_fd);
    }
}

//Receiving messages on our port
void receiving(int server_fd) {
    struct sockaddr_in address;
    int valread;
    char buffer[2000] = {0};
    int addrlen = sizeof(address);
    fd_set current_sockets, ready_sockets;

    //Initialize my current set
    FD_ZERO(&current_sockets);
    FD_SET(server_fd, &current_sockets);
    int k = 0;
    while (1) {
        k++;
        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == server_fd) {
                    int client_socket;
                    if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                                (socklen_t *)&addrlen)) < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    FD_SET(client_socket, &current_sockets);
                } else {
                    int msg_type = receive_msg(i);
                    if (msg_type == TEST_MSG) {
                        send_fault_status(i, FAULTY);
                    } else if (msg_type == REQUEST_MSG) {
                        send_array(i, tested_up, NUM_NODES);
                    }
                }
            }
        }

        if (k == (FD_SETSIZE * 2)) break;
    }
}

void update_arr(connection connections[], int num_connections, int node_num) {
  for (int i = 0; i < num_connections; ++i) {
    int sock = init_client_to_server(connections[i].ip_addr);
    if (sock < 0) {
        perror("Issue creating a socket\n");
        continue;
    }

    // Ask for fault status
    int fault_status = request_fault_status(sock);
    if (!fault_status) {
      int new_arr[NUM_NODES];
      request_arr(sock, new_arr);
      fault_status = request_fault_status(sock); // check fault status again before updating array
      close(sock);
      if (!fault_status) {
          update_tested_up(new_arr,node_num, connections[i].node_num); // todo: put actual node numbers
      }
      break;
    }

    close(sock);
  }  
}

void update_tested_up(int new_arr[], int node, int tested_node) {
    tested_up[node] = tested_node;
    for (int i = 0; i < NUM_NODES; ++i) {
        if (i < NUM_NODES - 1) printf("%i ", tested_up[i]);
        else printf("%i\n", tested_up[i]);
    }
    for (int i = 0; i < NUM_NODES; ++i) {
        if (i != node) {
          tested_up[i] = new_arr[i];
        }
    }
    for (int i = 0; i < NUM_NODES; ++i) {
        if (i < NUM_NODES - 1) printf("%i ", tested_up[i]);
        else printf("%i\n", tested_up[i]);
    }
}