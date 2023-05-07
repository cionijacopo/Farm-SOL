/*
    Created by Jacopo Cioni
    FarmProject - SOL
*/

#include"../../includes/utils.h"

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

void *s_malloc(size_t size) {
    void *p;
    if((p = malloc(size)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

int isNumber(const char* s, long* n) {
  if (s==NULL) return 1;
  if (strlen(s)==0) return 1;
  char* e = NULL;
  errno=0;
  long val = strtol(s, &e, 10);
  if (errno == ERANGE) return 2;    // overflow/underflow
  if (e != NULL && *e == (char)0) {
    *n = val;
    return 0;   // successo 
  }
  return 1;   // non e' un numero
}
