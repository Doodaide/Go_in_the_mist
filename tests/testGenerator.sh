#!/bin/bash



for i in $(seq -f "%03g" 21 25)
do
  read name

    touch ${i}${name}.in ${i}${name}.out 

done

