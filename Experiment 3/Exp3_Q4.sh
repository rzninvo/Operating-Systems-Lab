#!/bin/bash
counter=0
if [ -s $3 ]
then
	while IFS= read -r line
	do
		let counter=$counter+1
		if [ $counter -ge $1 ] && [ $counter -le $2 ]
		then
			echo "$line"
		fi	
	done < "$3"
fi
