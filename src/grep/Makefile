SHELL := /bin/bash
FLAG = -Wall -Wextra -Werror
FILE = 1.txt 2.txt
PATTERNS = patterns.reg
CHECK = 2>&1 valgrind --tool=memcheck --leak-ckeack=yes --leak-check=full --show-leak-kinds=all
ERROR = ERROR

all: s21_grep tests
s21_grep:
	gcc $(FLAG) grep.c -o s21_grep

tests:
	-diff <(./s21_grep He $(FILE)) <(grep He $(FILE))
	-diff <(./s21_grep -v He $(FILE)) <(grep -v He $(FILE))
	-diff <(./s21_grep -i He $(FILE)) <(grep -i He $(FILE))
	-diff <(./s21_grep -iv He $(FILE)) <(grep -iv He $(FILE))
	-diff <(./s21_grep -in He $(FILE)) <(grep -in He $(FILE))
	-diff <(./s21_grep -ie "^he" $(FILE)) <(grep ie "^he" $(FILE))
	-diff <(./s21_grep -ince "^he" $(FILE)) <(grep -ince "^he" $(FILE))
	-diff <(./s21_grep -lh se $(FILE)) <(grep -lh se $(FILE))
	-diff <(./s21_grep -hc e $(FILE)) <(grep -hc e $(FILE))
	
	-diff <(./s21_grep -e He -e he $(FILE)) <(grep He $(FILE))
	-diff <(./s21_grep -s he badfile) <(grep -s he badfile)
	-diff <(./s21_grep -f $(PATTERNS) $(FILE)) <(grep -f $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oi he $(FILE)) <(grep -oi he $(FILE))
	-diff <(./s21_grep -ohf $(PATTERNS) $(FILE)) <(grep -ohf $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oinc he $(FILE)) <(grep -oinc he $(FILE))
	-diff <(./s21_grep -oihlnc -e he -e Se -f patterns.reg $(FILE)) <(grep -oihlnc -e he -e Se -f patterns.reg $(FILE))


	$(CHECK) ./s21_grep he *.txt | grep -E $(ERROR) 
	$(CHECK) ./s21_grep -e He -e he $(FILE) | grep -E $(ERROR)
	$(CHECK) ./s21_grep -f $(PATTERNS) $(FILE) | grep -E $(ERROR)
