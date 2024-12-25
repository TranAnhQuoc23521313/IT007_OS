#! /bin/bash

echo "Nhap vao ten: "
read name

echo "Nhap vao MSSV: "
read MSSV

while [ "$MSSV" != "23521313" ]
do
    echo "Nhap sai MSSV roi !! Hay nhap lai"
    read MSSV
done 

echo "Xin chao $HoTen !"
exit 0