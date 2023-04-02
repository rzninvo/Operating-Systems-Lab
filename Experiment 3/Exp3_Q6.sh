#!/bin/bash
let tmp=0
if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]
then
        echo "ERROR: Not enough arguments. arguments should be 'value' 'operator' 'value'."
else
        case $2 in
                +)
			tmp=`echo "$1+$3" | bc`
                        echo "Result: $tmp"
                        ;;
                -)
                        tmp=`echo "$1-$3" | bc`
                        echo "Result: $tmp"
                        ;;
                /)
                        tmp=`echo "$1/$3" | bc -l`
                        echo "Result: $tmp"
                        ;;
                x)
                        tmp=`echo "$1*$3" | bc -l`
                        echo "Result: $tmp"
                        ;;
        esac
fi
