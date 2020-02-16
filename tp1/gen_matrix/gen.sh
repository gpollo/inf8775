#!/bin/bash

seq 2 11 | while read N; do
  for I in {1..3}; do
    echo ./Gen $N "ex_"$N"."$I
    ./gen $N "ex_"$N"."$I
  done
done
