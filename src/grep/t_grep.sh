#!/bin/bash

RED="$(tput setab 1)"
GREEN="$(tput setab 2)"
RESET="$(tput sgr0)"

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""

test_diff() {
  if [ "$1" == "" ]; then
    ((COUNTER_SUCCESS++))
    echo "$GREEN SUCCESS $RESET ./s21_grep $2"
  else
    ((COUNTER_FAIL++))
    echo "$RED FAIL $RESET ./s21_grep $2"
    echo "$DIFF_RES"
  fi
}

for var in -v -c -l -n -h -o; do
  for var2 in -v -c -l -n -h -o; do
    for var3 in -v -c -l -n -h -o; do
      if [ $var == $var2 ] || [ $var2 == $var3 ] || [ $var == $var3 ]; then
        continue
      fi

      TESTABLE="for s21_grep.c s21_grep.h Makefile $var $var2 $var3"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="for s21_grep.c $var $var2 $var3"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="-e for -e ^int s21_grep.c s21_grep.h Makefile $var $var2 $var3"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="-e for -e ^int s21_grep.c $var $var2 $var3"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="-e regex -e ^print s21_grep.c $var $var2 $var3 -f pattern.txt"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="-e while -e void s21_grep.c Makefile $var $var2 $var3 -f pattern.txt"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"

      TESTABLE="-e W -e or -e er -e rr Makefile -e Wa -e err $var $var2 $var3"
      ./s21_grep $TESTABLE >1.txt
      grep $TESTABLE >2.txt
      DIFF_RES="$(diff 1.txt 2.txt)"
      rm 1.txt 2.txt
      test_diff "$DIFF_RES" "$TESTABLE"
    done
  done
done

echo ""
echo "$GREEN SUCCEEDED $RESET: $COUNTER_SUCCESS"
echo "$RED FAILED $RESET: $COUNTER_FAIL"

exit "$COUNTER_FAIL"
