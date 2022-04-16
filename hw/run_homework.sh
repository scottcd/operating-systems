#!/bin/bash
# Developer:   	Chandler Scott
# Project:      Homework 2 - Part 1
# Created:   	02/24/2022
# Last Edit: 	03/31/2022

################################################################################    
# Help 
################################################################################    
Help()
{
    # Display Help
    echo "This bash script can help run Chandler's homework 3 with some different"
    echo "options to help with testing. Homework 3 specifications will be discussed"
    echo "below:"
    echo 
    echo "Create 5 child processes that will perform operations on each product"
    echo "record station. Once all records have been read, one final record with"
    echo "stations set to -1 will be piped to the child processes. At this point, "
    echo "the child processes will print the final statistics of how many times"
    echo "they ran."
    echo ""
    echo "options:"
    echo "-h        Print this help"
    echo "-a        Run all product records"
    echo "-f        Specify which record to use (0=all, 1=order.txt, "
    echo "                                       2=order1.txt, 3=order2.txt)"

}

################################################################################    
# RunOrders
################################################################################    
RunOrders()
{
    case "$1" in
    # all
    0)
	echo -e "\n-------------------------"
        echo "Reading from: ALL FILES"
        echo -e "-------------------------\n"
        echo -e "-------------------------\n"
        ./hw4 order.txt out1
        echo -e "-------------------------\n"
        echo -e "-------------------------\n"
        ./hw4 order1.txt out2
        echo -e "-------------------------\n"
        echo -e "-------------------------\n"
        ./hw4 order2.txt out3
    ;;
    # order.txt
    1)
        echo -e "\n-------------------------"
        echo "Reading from: order.txt"
        echo -e "-------------------------\n"
        ./hw4 order.txt out
    ;;
    # order2.txt
    2)
 	echo -e "\n-------------------------"
        echo "Reading from: order.txt"
        echo -e "-------------------------\n"
        ./hw4 order1.txt out
    ;;
    # order1.txt
    3) 
        echo -e "\n-------------------------"
        echo "Reading from: order.txt"
        echo -e "-------------------------\n"
        ./hw4 order2.txt out
    ;;
    esac
    shift
}

################################################################################    
# Main 
################################################################################    
# Check to see if we already compiled
if [ ! -f hw4 ]
then
    # Check for the existence of Makefile; if not there, quit with error, else run it
    if test -f "Makefile"; then
        make
    else
        echo "Makefile not found."
        exit 1
    fi
fi

# Second, check for the existence of hw1; if not there, quit with error
if test -f "hw4"; then
    echo
    while [ -n "$1" ]
    do
    case "$1" in
    -h) Help ;exit 1;;
    -a) RunOrders 0;exit 1;;
    -f) RunOrders $2;exit 1;;
    esac
    shift
    done
    
    # run help if we didnt get any other options
    Help
    
    
    
else
    echo "hw4 file not found."
    exit 1
fi
