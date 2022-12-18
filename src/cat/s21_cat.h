#include <fcntl.h>  // file's descryptors
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //

typedef struct cat_bash_flags {
  int numeric;
  int numeric_nonblank;
  int end_of_line;
  int tabulation;
  int symbol;
  int squezze;
  int count;
} cat_bash_flags;

void Cat_With_flags(cat_bash_flags *cbf, char argv[], int argc);
void Cat_Without_Args(int file_d);
void Argv_read(int argc, char *argv[]);
void Analys_Flags(cat_bash_flags *cbf, int argc, char *argv);
