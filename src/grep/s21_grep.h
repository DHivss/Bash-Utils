/*Опция	Описание
-i	Игнорировать регистр. Не делать различия между большими и маленькими
символами. Также можно задать опцией --ignore-case. -v	Инвертировать
соответствие. Обычно grep печатает строки, которые содержат соответствие. Эта
опция приводит к тому, что grep выводит каждую строку, которая не содержит
соответствия. Также можно использовать --invert-match.
-c	Печатать количество соответствий (или несоответствий, если указана опция
-v) вместо самих строк. Можно также указывать опцией --count.
-l	Вместо самих строк печатать имя каждого файла, который содержит
соответствие. Можно указать опцией --files-with-matches. -L	Как опция -l, но
печатает только имена файлов, которые не содержат совпадений. Другое имя опции
--files-withoutmatch.
-n	Добавление к началу каждой совпавшей строке номера строчки внутри файла.
Другое имя опции --line-number. -h	Для поиска по нескольким файлам,
подавлять вывод имени файла. Также можно указать опцией --no-filename.*/
#define _GNU_SOURCE_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>  //

typedef struct grep_bash_flags {
  /*`-e`, `-i`, `-v`, `-c`, `-l`, `-n`*/
  int eregex;        // шаблон для поиска
  int ignore_case;   // игнорирование регистра
  int invert_match;  // несовпадающие строки
  int count;  // количество выбранных строк на файл
  int line_number;  // номер строки с совпадением
  int match_files;  // названия совпадающих файлов
  int count_args;
  int e_count;
  // char *files;
} grep_bash_flags;

void grep_work_flags(grep_bash_flags *gbf, char *files[], char *pattern);
void regexargs(int argc, char *argv[], char c, grep_bash_flags *gbf,
               char *pattern, char *files[]);
void concatenation(char *pattern, char *string);
void files_array(char *argv[], int argc, grep_bash_flags *gbf, char *files[],
                 int count, char *pattern);
void end_of_output(char *string);
void grep_without_flags(char *argv[], int argc);
