# Developer:   	Chandler Scott
# Project:      Homework 2 - Part 1
# Created:   	02/24/2022
# Last Edit: 	02/24/2022

# First, check for the existence of Makefile; if not there, quit with error, else run it
if test -f "Makefile"; then
    make
else
    echo "Makefile not found."
    exit 1
fi

# Second, check for the existence of hw1; if not there, quit with error
if test -f "hw1"; then
    ./hw1 order.txt output.txt
else
    echo "hw1 file not found."
    exit 1
fi
