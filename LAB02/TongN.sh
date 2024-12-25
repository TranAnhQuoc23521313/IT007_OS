#! /bin/bash

echo "Nhap vao so nguyen N"
read N

sum=0

while [ $N -lt 10 ]
do 
    echo "Nhap lai so nguyen N"
    read N 
done

for ((i = 1; i <= N; i++))
do 
    sum=$((sum+i))
done 

echo "Tong = $sum"
exit 0
