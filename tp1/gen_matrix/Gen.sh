#!/bin/bash

seq 5 12 | while read N; do
  for I in {1..5}; do
    echo ./Gen $N "ex_"$N"."$I
    ./Gen $N "ex_"$N"."$I
  done
done
