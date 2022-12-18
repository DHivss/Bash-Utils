CC = gcc -Wall -Werror -Wextra
all: s21_grep
clean:
	rm -rf s21_grep

s21_grep:
	$(CC) s21_grep.c -o s21_grep

tests:
	bash grep_test_grep.sh

leaks:
	bash valgrind_tests.sh