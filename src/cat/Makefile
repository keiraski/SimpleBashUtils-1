SHELL= /bin/bash
FLAG = -Wall -Wextra -Werror
FILE = 1.txt
CHECK = leaks -atExit -- ./s21_cat

all: claing s21 tests

claing:
	clang-format -i *.c

s21:
	gcc $(FLAG) s21_cat.c -o s21_cat

tests:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -etsnb $(FILE)) <(cat -etsnb $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat --number $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat --number-nonblank $(FILE)) <(cat -b $(FILE))

	$(CHECK) ./s21_cat $(FILE)
	$(CHECK) ./s21_cat -n $(FILE)
	$(CHECK) ./s21_cat -e $(FILE)
	$(CHECK) ./s21_cat -t $(FILE)
	$(CHECK) ./s21_cat -b $(FILE)
	$(CHECK) ./s21_cat -s $(FILE)
	$(CHECK) ./s21_cat -etsnb $(FILE)