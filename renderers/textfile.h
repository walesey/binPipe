#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEXTFILE_H
#define TEXTFILE_H

typedef struct {
  unsigned char *data;
  int size;
} File;

#endif

File fileRead(char *fn);

int fileWrite(char *fn, File s);

char *textFileRead(char *fn);

int textFileWrite(char *fn, char *s);
