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
#include <dirent.h>
#include "constants.h"
#include "communication.h"
#include "diagnose.h"
#include "entrophy.h"
#include "monitor.h"

void start_algo(int faulty, connection connections[], int num_connections, int node_num);
void adaptive_dsd(int faulty, connection connections[], int num_connections, int node_num, file_entr lookup[], int num_files);
void update_arr(connection connections[], int num_connections, int node_num);
void * receive_thread(void * server_fd);
void receiving(int server_fd);
void update_tested_up(int new_arr[], int node, int tested_node);
void init_files(file_entr * files, int * size);

#endif
