SHELL := /bin/bash
FLAG = -Wall -Wextra -Werror
FILE = 11.txt 22.txt
PATTERNS = pattern.txt
CHECK = leaks -atExit --

all: claing build tests

claing:
	clang-format -i *.c

build:
	gcc $(FLAG) s21_grep.c -o s21_grep

tests:
	-diff <(./s21_grep He $(FILE)) <(grep He $(FILE))
	-diff <(./s21_grep -e He $(FILE)) <(grep -e He $(FILE))
	-diff <(./s21_grep -i He $(FILE)) <(grep -i He $(FILE))
	-diff <(./s21_grep -v He $(FILE)) <(grep -v He $(FILE))
	-diff <(./s21_grep -c He $(FILE)) <(grep -c He $(FILE))
	-diff <(./s21_grep -ic He $(FILE)) <(grep -ic He $(FILE))
	-diff <(./s21_grep -iv He $(FILE)) <(grep -iv He $(FILE))
	-diff <(./s21_grep -in He $(FILE)) <(grep -in He $(FILE))
	-diff <(./s21_grep -is He $(FILE)) <(grep -is He $(FILE))
	-diff <(./s21_grep -ie "^he" $(FILE)) <(grep -ie "^he" $(FILE))
	-diff <(./s21_grep -ince "^he" $(FILE)) <(grep -ince "^he" $(FILE))
	-diff <(./s21_grep -lh se $(FILE)) <(grep -lh se $(FILE))
	-diff <(./s21_grep -hc he $(FILE)) <(grep -hc he $(FILE))
	-diff <(./s21_grep -ho He $(FILE)) <(grep -ho He $(FILE))
	-diff <(./s21_grep -e He -e he $(FILE)) <(grep -e He -e he $(FILE))
	-diff <(./s21_grep -s he badfile) <(grep -s he badfile)
	-diff <(./s21_grep -f $(PATTERNS) $(FILE)) <(grep -f $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oi he $(FILE)) <(grep -oi he $(FILE))
	-diff <(./s21_grep -ohf $(PATTERNS) $(FILE)) <(grep -ohf $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oinc he $(FILE)) <(grep -oinc he $(FILE))

mem:
	$(CHECK) ./s21_grep he *.txt
	$(CHECK) ./s21_grep -e He -e he $(FILE)
	$(CHECK) ./s21_grep -f $(PATTERNS) $(FILE)

clean:
	rm s21_grep