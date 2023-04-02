#!/bin/bash
input=0
while [ $input -ne -1 ]
do
	read input
	if [ $input -eq -1 ]
	then
		break
	fi
	tmp=$input
	tmp2=0
	sum=0
	while [ $tmp -ne 0 ]
	do
		tmp3=$( expr $tmp % 10 )
		let sum=$sum+$tmp3
		tmp4=$( expr $tmp2 \* 10 )
		tmp2=$( expr $tmp4 + $tmp3 )
		tmp=$( expr $tmp / 10 )	
	done
	echo "Inversed Number: $tmp2"
	echo "Sum of digits: $sum"
done
