CC = gcc -Wall -Werror -Wextra
all: s21_cat
clean:
	rm -rf s21_cat

s21_cat:
	$(CC) s21_cat.c s21_cat.h -o s21_cat
tests:
	bash cat_tests.sh