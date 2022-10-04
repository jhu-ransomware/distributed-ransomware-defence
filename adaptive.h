#ifndef ADAPTIVE_H
#define ADAPTIVE_H

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
#include "constants.h"
#include "communication.h"

void start_algo(int faulty, char ips[][IP_LENGTH], int num_connections);
void adaptive_dsd();
void check_status(char ips[][IP_LENGTH], int num_connections);
void * receive_thread(void * server_fd);
void receiving(int server_fd);

#endif