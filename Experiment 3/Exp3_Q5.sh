#!/bin/bash
echo "Please enter line count:"
read input
case $1 in
	1)
	for (( i=0; i<$input; i++ ))
	do  
		for (( j=0; j<=i; j++ ))
		do
			if [ $j -eq $i ]
			then
				if [ $i -ne 0 ]
				then
					echo " |_"
				else
					echo "|_"
				fi
			else
				if [ $j -eq 0 ]
				then
					echo -n "|"
				else
					echo -n " |"
				fi
			fi	
		done
	done
	;;
	2)
		for (( i=1; i<=$input; i++ ))
		do
			let insp=$i-1
			mu=$( expr $input \* 2 )
			let sp=$( expr $( expr $( expr $mu - $i ) - $insp ) / 2 ) 
			for (( k=1; k<=sp; k++ ))
			do
				echo -n " "
			done
			for (( j=1; j<=i; j++ ))
			do 
				echo -n "* "
			done
			for (( k=1; k<=sp; k++ ))
                        do
                                echo -n " "
                        done
			echo ""
		done		
		for (( i=$input; i>=1; i-- ))
                do
			let insp=$i-1
                        mu=$( expr $input \* 2 )
                        let sp=$( expr $( expr $( expr $mu - $i ) - $insp ) / 2 )
                        for (( k=1; k<=sp; k++ ))
                        do
                                echo -n " "
                        done
                        for (( j=1; j<=i; j++ ))
                        do
                                echo -n "* "
                        done
                        for (( k=1; k<=sp; k++ ))
                        do
                                echo -n " "
                        done
                        echo ""

                done
	;;
	3)
		for (( i=1; i<=$input; i++ ))
		do
			for (( j=1; j<=i; j++ ))
			do
				echo -n "$i"
			done
			echo ""
		done
esac
