#ifndef CONSTANTS_H
#define CONSTANTS_H

#define IP_LENGTH 16
#define PORT 10100
#define TESTING_INTERVAL 5
#define NUM_NODES 2

#define DEMO 1

#define TEST_MSG 1
#define REQUEST_MSG 2

typedef struct {
  char ip_addr[IP_LENGTH];
  int node_num;
} connection;

#endif
