#include "s21_grep.h"

int main(int argc, char *argv[]) {
  grep_bash_flags gbf = {0, 0, 0, 0, 0, 0, 0, 0};
  char pattern[1000] = {0};
  char *files[1000] = {NULL};
  if (argc <= 2) {
    printf("Использование: [ПАРАМЕТР]... ШАБЛОНЫ [ФАЙЛ]...\n");
  } else {
    char c = '.';
    regexargs(argc, argv, c, &gbf, pattern, files);
  }
  return 0;
}

void regexargs(int argc, char *argv[], char c, grep_bash_flags *gbf,
               char *pattern, char *files[]) {
  int count_flags = 0;
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] == '-') count_flags++;
  }
  if (count_flags == 0) {
    grep_without_flags(argv, argc);
  } else {
    while ((c = getopt(argc, argv, "ivclne:")) != -1) {
      switch (c) {
        case 'i':
          gbf->ignore_case = 1;
          break;
        case 'v':
          gbf->invert_match = 1;
          break;
        case 'e':
          gbf->eregex = 1;
          concatenation(pattern, optarg);
          break;
        case 'l':
          gbf->match_files = 1;
          break;
        case 'n':
          gbf->line_number = 1;
          break;
        case 'c':
          gbf->count = 1;
          break;
      }
    }
    files_array(argv, argc, gbf, files, optind, pattern);
  }
}

void grep_without_flags(char *argv[], int argc) {
  FILE *file = NULL;
  char string[4096];
  regex_t ret;
  int result = 0;
  int check_match = 0;
  int e = 0;
  for (int i = 2; i < argc; i++) {
    if ((file = fopen(argv[i], "rb")) == NULL) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      continue;
    } else {
      while (fgets(string, 4096, file) != NULL) {
        if ((result = regcomp(&ret, argv[1], 0)) == 0) {
          check_match = regexec(&ret, string, 0, NULL, 0);
        } else {
          fprintf(stderr, "\n");
        }
        if (check_match == 0 && strcmp(string, "\n") != 0) {
          if (argc > 3) {
            printf("%s:%s", argv[i], string);
          } else {
            printf("%s", string);
          }
          e++;
        }
      }
      if (e >= 1 && *string == EOF) end_of_output(string);
    }
    regfree(&ret);
    fclose(file);
  }
}

void concatenation(char *pattern, char *string) {
  int i = strlen(string);
  if (string[i - 1] == '\0') {
    string[i - 1] = '|';
    strcat(pattern, string);
  } else {
    strcat(pattern, string);
    strcat(pattern, "|");
  }
}
void files_array(char *argv[], int argc, grep_bash_flags *gbf, char *files[],
                 int count, char *pattern) {
  if (gbf->eregex == 1) {
    while (count < argc && argv[count][0] != '-') {
      files[gbf->count_args] = argv[count];
      count++;
      gbf->count_args++;
    }
    int e = strlen(pattern);
    if (pattern[e - 1] == '|') pattern[e - 1] = '\0';
  } else {
    pattern = argv[count];
    count += 1;
    while (count < argc && argv[count][0] != '-') {
      files[gbf->count_args] = argv[count];
      count++;
      gbf->count_args++;
    }
  }
  grep_work_flags(gbf, files, pattern);
}

void grep_work_flags(grep_bash_flags *gbf, char *files[], char *pattern) {
  FILE *file = NULL;
  char string[4096] = "";
  int counter = 0;
  int line_count = 0;
  regex_t ret;
  int result = 0;
  int check_match = 0;
  int e_flag = 0;
  if (gbf->ignore_case == 1) {
    e_flag = REG_EXTENDED | REG_ICASE;
  } else if (gbf->eregex == 1) {
    e_flag = REG_EXTENDED;
  }
  for (int i = 0; i < gbf->count_args; i++) {
    if ((file = fopen(files[i], "rb")) == NULL) {
      fprintf(stderr, "grep: %s: No such file or directory\n", files[i]);
      continue;
    } else {
      while (fgets(string, 4096, file) != NULL) {
        line_count++;
        if ((result = regcomp(&ret, pattern, e_flag)) == 0) {
          check_match = regexec(&ret, string, 0, NULL, 0);
        } else {
          fprintf(stderr, "\n");
        }
        if (gbf->invert_match == 1) {
          if (check_match != 0) {
            if (gbf->match_files == 1 || gbf->count == 1) {
              counter++;
              continue;
            }
            if (gbf->line_number == 1) {
              if (gbf->count_args <= 1) {
                printf("%d:%s", line_count, string);
              } else {
                printf("%s:%d:%s", files[i], line_count, string);
              }
              end_of_output(string);
              continue;
            }
            if (gbf->count_args <= 1) {
              printf("%s", string);
            } else {
              printf("%s:%s", files[i], string);
            }
            end_of_output(string);
          }
          // end_of_output(string);
          // if (*string == EOF) {
          // printf("\n");
          //}
        }
        if (check_match == 0 && gbf->invert_match == 0) {
          if (gbf->match_files == 1 || gbf->count == 1) {
            gbf->line_number = 0;
            if (strcmp(string, "\n")) {
              counter++;
            }
            continue;
          }
          if (gbf->line_number == 1 && strcmp(string, "\n") != 0) {
            if (gbf->count_args <= 1) {
              printf("%d:%s", line_count, string);
            } else {
              printf("%s:%d:%s", files[i], line_count, string);
            }
            end_of_output(string);
            continue;
          }
          if (gbf->count_args <= 1) {
            printf("%s", string);
          } else {
            printf("%s:%s", files[i], string);
          }
          end_of_output(string);
        }
      }
      if (gbf->match_files == 1) {
        gbf->count = 0;
        if (counter != 0) {
          printf("%s\n", files[i]);
        }
      }
      if (gbf->count == 1) {
        if (gbf->count_args == 1) {
          printf("%d\n", counter);
        } else {
          printf("%s:%d\n", files[i], counter);
        }
      }
      regfree(&ret);
      line_count = 0;
      counter = 0;
      fclose(file);
    }
  }
}

void end_of_output(char *string) {
  int e = strlen(string);
  if (((string[e] == '\0' && string[e - 1] != '\n')) || *string == EOF) {
    printf("\n");
  }
}
