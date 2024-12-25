#! /bin/sh
myvar="Hi there"

echo $myvar
echo "message : $myvar"
echo 'message : $myvar'
echo "message : \$myvar"

echo Enter some text
read myvar

echo '$myvar' now equal $myvar

exit 0

#Running command: ~/LAB02/variables.sh