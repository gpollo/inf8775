#!/bin/bash

get_algo_cmd() {
    local algo=$1

    case "$algo" in
    glouton)
        printf "%s" "--greedy"
        ;;
    dp)
        printf "%s" "--dynamic"
        ;;
    heuristique)
        printf "%s" "--search"
        ;;
    esac
}

ALGO=("--greedy")
SONG=()
DO_PRINT_TIME=()
DO_PRINT_SOLUTION=()
DO_PRINT_COST=()

while [[ $# -gt 0 ]]; do
    case "$1" in
    -a)
        ALGO=("$(get_algo_cmd "$2")")
        shift
        ;;
    -e)
        SONG=("--song" "$2")
        shift
        ;;
    -t)
        DO_PRINT_TIME=("--benchmark")
        ;;
    -c)
        DO_PRINT_SOLUTION=("--solution")
        ;;
    -p)
        DO_PRINT_COST=("--cost")
        ;;
    *)
        echo "Unknown argument: ${1}"
        exit 1
    ;;
    esac
    shift
done

./bin/piano "${ALGO[@]}" "${SONG[@]}" "${DO_PRINT_SOLUTION[@]}" "${DO_PRINT_COST[@]}" "${DO_PRINT_TIME[@]}"
