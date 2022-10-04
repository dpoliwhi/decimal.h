CC=gcc
LIB=ar rcs
FLAGS=#-Werror -Wall -Wextra -std=c11
LINUX=-lrt -lpthread -lm -D_GNU_SOURCE
OS=$(shell uname -s)
CFLAGS=-fprofile-arcs -ftest-coverage

all: clean test

s21_decimal.a: s21_decimal.o
	$(LIB) s21_decimal.a s21_decimal.o
	cp s21_decimal.a libs21_decimal.a
	ranlib libs21_decimal.a

s21_decimal.o:
	$(CC) $(FLAGS) -c s21_decimal.c -o s21_decimal.o

test: s21_decimal.a
ifeq ($(OS), Darwin)
	$(CC) test_s21_decimal.c s21_decimal.a -o test -lcheck
	./test
else
	$(CC) test_s21_decimal.c s21_decimal.a -o test -lcheck $(LINUX)
	./test
endif

gcov_report: s21_decimal.a
	$(CC) $(CFLAGS) test_s21_decimal.c s21_decimal.c s21_decimal.a -o test -lcheck
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

open:
	open report/index.html

clean:
	rm -rf *.o *.out *.a *.gcda *.gcno *.info s21_decimal test
	rm -rf ./report/

check:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	python3 misc/cpplint.py --extensions=c *.c *.h
	CK_FORK=no leaks --atExit -- ./test
	make clean
