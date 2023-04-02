#!/bin/bash
let a=$1+$2
echo $a
if [ -z "$1" ]
then
	echo "ERROR: First argument not found"
fi
if [ -z "$2" ]
then
	echo "ERROR: Second argument not found"
fi
if [ -n "$1" ]&&[ -n "$2" ]
then
	if [ $1 -gt $2 ] 
	then
		echo "$1 is greater than $2"
	elif [ $2 -gt $1 ] 
	then
		echo "$2 is greater than $1"
	fi
fi
