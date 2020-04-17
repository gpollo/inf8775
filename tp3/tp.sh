#!/bin/bash

DATASET=("--dataset" "exemplaires/10_30_30_0.txt")
PROPAGATION=()
PRINT_SOLUTIONS=()

while [[ $# -gt 0 ]]; do
    case "$1" in
    -e)
        DATASET=("--dataset" "$2")
        shift
        ;;
    -k)
        PROPAGATION=("--virality" "$2")
        shift
        ;;
    -p)
        PRINT_SOLUTIONS=("--solutions")
        ;;
    *)
        echo "Unknown argument: ${1}"
        exit 1
    ;;
    esac
    shift
done

./build/pandemic "${DATASET[@]}" "${PROPAGATION[@]}" "${PRINT_SOLUTIONS[@]}"
