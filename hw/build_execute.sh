# Developer:   	Chandler Scott
# Project:      Homework 2 - Part 1
# Created:   	02/24/2022
# Last Edit: 	03/31/2022


# First, check for the existence of Makefile; if not there, quit with error, else run it
if test -f "Makefile"; then
    make
else
    echo "Makefile not found."
    exit 1
fi

# Second, check for the existence of hw1; if not there, quit with error
if test -f "hw3"; then
    
    echo -e "\n-------------------------"
    echo "Reading from: order.txt"
    echo -e "-------------------------\n"
    ./hw3 order.txt out

    echo -e "\n-------------------------"
    echo "Reading from: order1.txt"
    echo -e "-------------------------\n"
    ./hw3 order1.txt out1

    echo -e "\n-------------------------"
    echo "Reading from: order2.txt"
    echo -e "-------------------------\n"
    ./hw3 order2.txt out2
else
    echo "hw3 file not found."
    exit 1
fi
