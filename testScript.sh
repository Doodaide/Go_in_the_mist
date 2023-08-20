#!/bin/bash

count=0

for file in `ls tests/*.in`; do
    name=${file%.*}
    echo Running test $name
    in_file=${name}.in
    #cat $in_file
    #printf "\nOut File:"
    out_file=${name}.out
    
    # python3 run.py $config_file < $in_file | diff - $expected_file || echo "Test $name: failed!\n"
    ./gitm < $in_file | diff - $out_file || echo "Test $name: failed"
    printf "\n\n"
    count=$((count+1))
done

#./go < tests/input/001BlackWins.in | diff tests/output/001BlackWins.out - || echo "Test $name: failed"

echo "Finished running $count tests!"