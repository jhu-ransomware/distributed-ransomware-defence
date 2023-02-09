#include "hash.h"

// From: https://gist.github.com/sgsfak/9ba382a0049f6ee885f68621ae86079b#file-hashes-c-L27
int hash(const char* s, int len) {
    int h = 0;
    while (len) {
        h = 31 * h + *s++;
        --len;
    }
    return h;
}