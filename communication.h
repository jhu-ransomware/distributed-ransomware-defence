#ifndef COMMUNICATION_H
#define COMMUNICATION_H

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
#include "hash.h"

void send_array(int sock, int arr[], int arr_size);
void send_fault_status(int sock, int faulty);
int receive_msg(int sock);
int init_client_to_server(char ip_address[]);
int request_fault_status(int sock);
void request_arr(int sock, int arr[]);
void send_msg_to_demo_node(char ip_address[], int node_num, int arr[], int arr_size);

#endif