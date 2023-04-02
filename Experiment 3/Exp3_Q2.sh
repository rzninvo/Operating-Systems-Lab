#!/bin/bash
let tmp=0
if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]
then
	echo "ERROR: Not enough arguments. arguments should be 'value' 'operator' 'value'."
else
	case $2 in
		+)
			let tmp=$1+$3;
			echo "Result: $tmp"
			;;
		-)
			let tmp=$1-$3;
			echo "Result: $tmp"
			;;
		/)
			tmp=$( expr $1 / $3 )
			echo "Result: $tmp"
			;;
		x)
			let tmp=$( expr $1 \* $3 )
			echo "Result: $tmp"
			;;
	esac
fi
