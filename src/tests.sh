note=0
total=0

BASEDIR=$(dirname "$0")
correct_tests= $BASEDIR/../tests/correct/*.tpc
incorrect_tests=$BASEDIR/../tests/incorrect/*.tpc

AS=$BASEDIR/../as

printf "\nCORRECT\n\n"
echo " CORRECT " > test_results
echo " " >> test_results
for f in $correct_tests
do	
	let total++
	./../as < $f
	x=$($AS < $f)
	result=$?
	if [ $result -eq 0 ]
	then
		printf "$f \x1b[32m processed successfully\n \x1b[0m"
		echo "[ $f processed successfully ]" >> test_results
		let note++
	else
		printf "$f \x1b[31m error processing\n \x1b[0m"
		echo "[ $f ERROR : ] $x" >> test_results
	fi
done

printf "\nINCORRECT\n\n"

echo " " >> test_results
echo "---------------------" >> test_results
echo " " >> test_results
echo " INCORRECT " >> test_results
echo " " >> test_results

for f in $incorrect_tests
do
	echo " " >> test_results
	let total++
	x=$($AS < $f)
	result=$?
	if [ $result -ne 0 ]
	then
		printf "$f \x1b[32m processed successfully\n \x1b[0m"
		echo "[ $f processed successfully ]" >> test_results
		echo "$x" >> test_results
		let note++
	else
		printf "$f \x1b[31m error processing\n \x1b[0m"
		echo "[ $f ERROR CORRECT RETURNED | EXPECTED INCORRECT ]" >> test_results
	fi
done


echo " " >> test_results
echo "---------------------" >> test_results
echo " " >> test_results
echo "NOTE : $note / $total" >> test_results
printf "\n NOTE : $note / $total\n"
printf " (detailed output in test_results)\n"
