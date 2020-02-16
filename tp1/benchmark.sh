#!/bin/bash

function generate_matrix() {
    local dimension=$1
    local output=$2

    rm -f "$output"
    ./bin/generate_matrix "$dimension" "$output" > /dev/null
}

function benchmark_matrix() {
    local matrix=$1
    local algorithm=$2

    go run main.go --time "$algorithm" -a "$matrix" -b "$matrix"
}

function benchmark_power() {
    local power=$1
    local count=$2
    local algorithm=$3

    local results=()
    local matrix
    local sum

    matrix=$(mktemp -u)
    for i in $(seq 1 "$count"); do
        generate_matrix "$power" "$matrix"
        results+=("$(benchmark_matrix "$matrix" "$algorithm")")
    done

    sum=$(IFS="+"; bc <<< "${results[*]}")
    printf "%2.5f," $(python -c "print($sum/$count)")
}

function benchmark_algorithm() {
    local min_dim=$1
    local max_dim=$2
    local count=$3
    local algorithm=$4

    for power in $(seq "$min_dim" "$max_dim"); do
        benchmark_power "$power" "$count" "$algorithm"
    done
}

function benchmark() {
    local min_dim=$1
    local max_dim=$2
    local count=$3

    printf "naive,"
    benchmark_algorithm "$min_dim" "$max_dim" "$count" "--naive"
    printf "\n"

    #printf "strassen,"
    #benchmark_algorithm "$min_dim" "$max_dim" "$count" "--strassen"
    #printf "\n"
}

benchmark 2 10 3
