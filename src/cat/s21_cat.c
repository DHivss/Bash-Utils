#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc == 1)
    Cat_Without_Args(STDIN_FILENO);
  else
    Argv_read(argc, argv);
  return 0;
}

void Cat_Without_Args(int file_d) {
  char array[4096];
  // int file_d = STDIN_FILENO; =1 файловый дескриптор, прямой вызов системных
  // функций read, write, select и пр.
  if (file_d == -1) {
    printf("file not found");
    return;
  }
  int bytes_read;
  bytes_read = read(file_d, array, 4096);
  while (bytes_read != -1) {
    printf("%.*s", bytes_read, array);
    bytes_read = read(file_d, array, 4096);
  }
}
void Cat_With_flags(cat_bash_flags *cbf, char argv[], int argc) {
  (void)argc;
  FILE *f = fopen(argv, "rb");
  char c;
  int begin_str = 0;
  int is_empty = 0;
  while ((c = fgetc(f)) != EOF) {
    if (cbf->numeric_nonblank == 1) {
      cbf->numeric = 0;
    }
    if (cbf->squezze == 1) {
      if (c == '\n') {
        is_empty++;
      }
      if (is_empty > 2 && c == '\n') {
        is_empty--;
        continue;
      }
      if (c != '\n') {
        is_empty = 0;
      }
    }
    if (cbf->numeric == 1) {
      if (begin_str == 0) {
        printf("%6d\t", cbf->count++);
      }
    }
    begin_str++;

    if ((c == '\n') && begin_str != 0) {
      begin_str = 0;
    }
    if (cbf->numeric_nonblank == 1 && begin_str == 1) {
      if (c == '\n') {
        printf("%s\t", "");
      } else {
        printf("%6d\t", cbf->count++);
      }
    }

    if (cbf->tabulation == 1) {
      if (c == '\t') {
        putchar('^');
        c = 'I';
      }
    }
    if (cbf->end_of_line == 1 && c == '\n') {
      putchar('$');
    }
    if (cbf->symbol == 1) {
      if (c < 32 && c != 10 && c != 9 && c >= 0) {
        putchar('^');
        c = c + 64;
      }
      if (c == 127) {
        putchar('^');
        c = c - 64;
      }
    }
    putchar(c);
  }
  fclose(f);
}

void Argv_read(int argc, char *argv[]) {
  cat_bash_flags cbf = {0, 0, 0, 0, 0, 0, 0};
  for (int i = 1; i != argc; i++) {
    if (argv[i][0] == '-') {
      Analys_Flags(&cbf, argc, argv[i]);
    }
  }
  for (int j = 1; j != argc; j++) {
    if (argv[j][0] != '-') {
      Cat_With_flags(&cbf, argv[j], argc);
    }
  }
}

void Analys_Flags(cat_bash_flags *cbf, int argc, char *argv) {
  cbf->count = 1;
  (void)argc;
  argv++;
  if (argv[0] == '-') {
    // argv++;
    if (strcmp(argv, "-number-nonblank") == 0) {
      cbf->numeric_nonblank = 1;
    } else if (strcmp(argv, "-number") == 0) {
      cbf->numeric = 1;
    } else if (strcmp(argv, "-squezze-blank") == 0) {
      cbf->squezze = 1;
    } else if (strcmp(argv, "-show-tabs") == 0) {
      cbf->tabulation = 1;
    } else {
      printf("unknown option -- %s\n", argv);
    }
  } else {
    for (char *it = argv; *it != *"\0"; it++) {
      switch (*it) {
        case 'b':
          cbf->numeric_nonblank = 1;
          break;
        case 'n':
          cbf->numeric = 1;
          break;
        case 'T':
          cbf->tabulation = 1;
          break;
        case 't':
          cbf->tabulation = 1;
          cbf->symbol = 1;
          break;
        case 'v':
          cbf->symbol = 1;
          break;
        case 'e':
          cbf->end_of_line = 1;
          cbf->symbol = 1;
          break;
        case 'E':
          cbf->end_of_line = 1;
          break;
        case 's':
          cbf->squezze = 1;
          break;
        default:
          fprintf(stderr, "unknow option -- %s\n", it);
          exit(0);
      }
    }
  }
}
