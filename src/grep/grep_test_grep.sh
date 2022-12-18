#!/bin/bash
SUCCESS=0
ERROR=0
COUNTER=0
DIFF=""
FILES=("test_1_grep.txt" "test_2_grep.txt" "test_3_grep.txt")
test() {
    for ((i=0; i<${#FILES[@]}; i++))
    do
    grep $var1 ${FILES[$i]} > check.txt
    ./s21_grep $var1 ${FILES[$i]} > check2.txt
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "$(diff check.txt check2.txt)" ]
    then
    (( SUCCESS++ ))
    echo "Success test #$COUNTER ./s21_grep $var1 ${FILES[$i]}"
    else
    (( ERROR++ ))
    echo "Fail test #$COUNTER ./s21_grep $var1 ${FILES[$i]}"
    fi
    done
    rm check.txt check2.txt
}

test2()
{
    for ((i=0; i<${#FILES[@]}; i++))
    do
    grep $var4 ${FILES[$i]} $var1 > check.txt
    ./s21_grep $var4 ${FILES[$i]} $var1 > check2.txt
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "$(diff check.txt check2.txt)" ]
    then
    (( SUCCESS++ ))
    echo "Success test #$COUNTER ./s21_grep $var4 ${FILES[$i]} $var1"
    else
    (( ERROR++ ))
    echo "Fail test #$COUNTER ./s21_grep $var4 ${FILES[$i]} $var1"
    fi
    done
    rm check.txt check2.txt
}

for var in 'che.' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
    var1="$var"
    test $var1
done

for var in 'che.' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
    for var3 in "test_1_grep.txt" "test_2_grep.txt" "test_3_grep.txt"
    do
    var1="$var $var3"
    test $var1
    done
done

for var in e v i c l n
do
    for var2 in '^S' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
    var1="-$var $var2"
    test $var1
    done
done

for var in v i c l n
do
    for var2 in '^S' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
    var1="-$var -e $var2"
    test $var1
    done
done

for var in e v i c l n
do
    for var2 in '^S' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
        for var3 in "test_1_grep.txt" "test_2_grep.txt" "test_3_grep.txt"
        do
        var1="-$var $var2 $var3"
        test $var1
        done
    done
done

for var in v i c l n 
do
    for var2 in v i c l n e
    do
        for var3 in '^S' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
        do
        if [ $var != $var2 ]
        then
        var1="-$var$var2 $var3"
        test $var1
        fi
        done
    done
done

for var in '^s' 'int' 'FRiend'
do
    for var2 in '^S' 'int' '[А-Яа-яЁё]\.txt' 'FRiend'
    do
    if [ $var != $var2 ]
    then
    var1="-e $var -e $var2"
    test $var1
    fi
    done
done

for var4 in '^s' 'int' 'FRiend' '[А-Яа-яЁё]\.txt'
do
    for var2 in v i c l n 
    do
        for var3 in v i c l n 
        do
        if [ $var2 != $var3 ]
        then
        var1="-$var2 -$var3"
        test2 $var4, $var1
        fi
        done
    done
done

echo "Succesful test: $SUCCESS Fail tests: $ERROR"
