#!/bin/bash

COUNTER=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )
flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "h"
    "s"
    "e"
)
tests=(
"FLAGS Test test_files/test_1_grep.txt test_files/test_2_grep.txt"
)
manual=(
"-e o -e test test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e hello -f test_files/test_3_grep.txt test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-s test do_not_exist.txt"
)
run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        echo "$COUNTER - Success $param"
    else
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done