#! /bin/bash

echo "Nhap chuoi"

read String

for file in *
do 
    if grep -l "$String" $file 
    then 
        more $file
    fi 
done 

exit 0