#!/bin/bash

get_algo_cmd() {
    local algo=$1

    case "$algo" in
    conv)
        printf "--naive"
        ;;
    strassen)
        printf "--strassen"
        ;;
    strassenSeuil)
        printf "--hybrid"
        ;;
    esac
}

ALGO=("--strassen")
MATRIX_A=()
MATRIX_B=()
DO_PRINT=()
DO_TIME=()

while [[ $# -gt 0 ]]; do
    case "$1" in
    -a)
        ALGO=("$(get_algo_cmd "$2")")
        shift
        ;;
    -e1)
        MATRIX_A=("--matrix-a" "$2")
        shift
        ;;
    -e2)
        MATRIX_B=("--matrix-b" "$2")
        shift
        ;;
    -p)
        DO_PRINT=("--print")
        ;;
    -t)
        DO_TIME=("--time")
        ;;
    *)
        echo "Unknown argument: ${1}"
        exit 1
    ;;
    esac
    shift
done

go run ./*.go "${ALGO[@]}" "${MATRIX_A[@]}" "${MATRIX_B[@]}" "${DO_PRINT[@]}" "${DO_TIME[@]}"
