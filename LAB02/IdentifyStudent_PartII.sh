#! /bin/bash

# Dùng while
echo "Nhap vao ten: "
read name

echo "Nhap vao MSSV: "
read MSSV

while [ "$MSSV" != "23521313" ] || [ "$name" != "Tran Anh Quoc" ]
do  
    echo "Please log in again !"

    echo "Nhap vao ten: "
    read name

    echo "Nhap vao MSSV: "
    read MSSV
done

echo "Welcome back $name"

echo "Dung For loop"

# Dùng for
for ((index = 1; index <= 5; index++))
do
    echo "Please log in"

    echo "Nhap vao ten: "
    read name

    echo "Nhap vao MSSV: "
    read MSSV
    if [ "$MSSV" = "23521313" ] && [ "$name" = "Tran Anh Quoc" ]
    then 
        Check="true"
        break
    fi 
done

if [ "$Check" = "true" ]
then
    echo "Welcome $name come back"
else
    echo "Login failed!"
fi
