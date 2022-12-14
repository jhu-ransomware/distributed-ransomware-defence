#include "constants.h"
#include "stdio.h"
#include "monitor.h"
#include <string.h>
#include <unistd.h>

int main() {

  printf("Starting monitoring\n");

  file_entr begin;
  strcpy(begin.filename, "test.txt");
  begin.entrophy = 4.5; // Test first value should be close enough

  file_entr tests[1];
  tests[0] = begin;

  while (1) {
    if (update_entrophy(tests, 1)) {
        break;
    }
    printf("no change\n");
    sleep(2);
  }

  printf("Change in entrophy detected\n");

  return 1;
}