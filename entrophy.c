/*
This code is adapted from https://github.com/robvandenbrink/Ransomware-Scan-and-Replicate/blob/V1.0/entropy.c
*/
#include "entrophy.h"

// not included in some cases
double log2( double n ) {  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}  

int makehist(FILE *fh, int *hist, int len) {
  int wherechar[256];
  int i, j, histlen, buflen;
  unsigned char c[102400]; /* define a reasonable buffer to read the file - 1 byte at a time is too slow  */
  histlen = 0;
  for (i = 0; i < 256; i++) {
    wherechar[i] = -1;
  }
  for (i = 0; i < len; ) {
    buflen = fread(&c, sizeof(unsigned char), 102400, fh);
    for (j = 0; j < buflen; j++)
    {
      if (wherechar[(unsigned int)c[j]] == -1)
      {
        wherechar[(unsigned int)c[j]] = histlen;
        histlen++;
      }
      ++i;
      hist[wherechar[(unsigned int)c[j]]]++;
    }
  }
  return histlen;
}

double entropy(int *hist, int histlen, int len) {
  int i;
  double H;
  H = 0;
  for (i = 0; i < histlen; i++)
  {
    H -= (double)hist[i] / len * log2((double)hist[i] / len);
  }
  return H;
}

double calc_entrophy_file(char * filename) {
  FILE *fh;
  struct stat fileinfo;
  long fsz;
  int len, *hist, histlen;
  double H;
  if ((fh = fopen(filename, "rb")) == NULL) {
    printf("Error opening file %s\n", filename);
  }
  else {
    stat(filename, &fileinfo);
    fsz = fileinfo.st_size;
  }
  hist = (int *)calloc(fsz, sizeof(int));
  histlen = makehist(fh, hist, fsz);
  // hist now has no order (known to the program) but that doesn't matter
  H = entropy(hist, histlen, fsz);
  fclose(fh);
  //printf("%lf\n", H);
  return H;
}