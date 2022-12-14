#include "constants.h"
#include "stdio.h"
#include "monitor.h"
#include <string.h>

int main() {


  file_entr begin;
  strcpy(begin.filename, "test.txt");
  begin.entrophy = 4.5;

  file_entr tests[1];
  tests[0] = begin;

  while (1) {
    if (update_entrophy(tests, 1)) {
        break;
    }
  }

  printf("Change in entrophy detected");

  return 1;
}