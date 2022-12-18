#!bin/bash

DIFF_RES=""
FILES=("test_1_cat.txt" "test_2_cat.txt" "test_3_cat.txt")
COUNTER=0
SUCCESS=0
ERROR=0

test()
{
    for ((i=0; i<${#FILES[@]}; i++))
    do
    cat $var1 ${FILES[$i]} > check.txt
    ./s21_cat $var1 ${FILES[$i]} > check2.txt
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "$(diff check.txt check2.txt)" ]
    then
    (( SUCCESS++ ))
    echo "Success test #$COUNTER ./s21_cat $var1 ${FILES[$i]}"
    else
    (( ERROR++ ))
    echo "Fail test #$COUNTER ./s21_cat $var1 ${FILES[$i]}"
    fi
    done
    rm check.txt check2.txt
}

var1="-z"
test $var1

for var in s t v n e b
do 
var1="-$var"
test $var1
done

for var in s t v n e b
do
    for var2 in "test_1_cat.txt" "test_2_cat.txt" "test_3_cat.txt"
    do
    var1="-$var $var2"
    test $var1
    done
done

for var in s t v n e b
do
    for var2 in s t v n e b
    do 
        if [ $var != $var2 ]
        then
        var1="-$var -$var2"
        test $var1
        fi
    done
done

for var in s t v n e b
do
    for var2 in s t v n e b
    do 
        for var3 in s t v n e b
        do
            if [ $var != $var2 ] && [ $var != $var3 ] && [ $var2 != $var3 ]
            then
            var1="-$var -$var2 -$var3"
            test $var1
            fi
        done
    done
done

echo "Succesful test: $SUCCESS Fail tests: $ERROR"
