#!/bin/bash

count_A_first=0
count_B_first=0
runs=100

for ((i=1; i<=runs; i++)); do
    sleep .25
    output=$(./build/threads)

    # Remove carriage returns, if any
    output=$(echo "$output" | tr -d '\r')

    # Extract just the relevant lines (A/B)
    order=$(echo "$output" | grep -E '^[AB]$' | tr -d '\n')

    if [[ "$order" =~ ^A ]]; then
        ((count_A_first++))
    elif [[ "$order" =~ ^B ]]; then
        ((count_B_first++))
    fi
done

echo "A was first: $count_A_first / $runs"
echo "B was first: $count_B_first / $runs"

