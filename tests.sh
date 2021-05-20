note=0
total=0

BASEDIR=$(dirname "$0")
good_tests=$BASEDIR/test/good/*.tpc
syn_err=$BASEDIR/test/syn-err/*.tpc

AS=$BASEDIR/bin/tpcc

OUTPUT=$BASEDIR/test_results

printf "\nCORRECT\n\n"
echo " CORRECT " > $OUTPUT
echo " " >> $OUTPUT
for f in $good_tests
do	
	let total++
	$AS < $f
	x=$($AS < $f)
	result=$?
	if [ $result -eq 0 ]
	then
		printf "$f \x1b[32m processed successfully\n \x1b[0m\n"
		echo "[ $f processed successfully ]\n" >> $OUTPUT
		let note++
	else
		printf "$f \x1b[31m error processing\n \x1b[0m"
		echo "[ $f ERROR : ] $x" >> $OUTPUT
	fi
done

printf "\nINCORRECT\n\n"

echo " " >> $OUTPUT
echo "---------------------" >> $OUTPUT
echo " " >> $OUTPUT
echo " INCORRECT " >> $OUTPUT
echo " " >> $OUTPUT

for f in $syn_err
do
	echo " " >> $OUTPUT
	let total++
	x=$($AS < $f)
	result=$?
	if [ $result -ne 0 ]
	then
		printf "$f \x1b[32m processed successfully\n \x1b[0m"
		echo "[ $f processed successfully ]" >> $OUTPUT
		echo "$x" >> test_results
		let note++
	else
		printf "$f \x1b[31m error processing\n \x1b[0m"
		echo "[ $f ERROR CORRECT RETURNED | EXPECTED INCORRECT ]" >> $OUTPUT
	fi
done


echo " " >> $OUTPUT
echo "---------------------" >> $OUTPUT
echo " " >> $OUTPUT
echo "NOTE : $note / $total" >> $OUTPUT
printf "\n NOTE : $note / $total\n"
printf " (detailed output in test_results)\n"
