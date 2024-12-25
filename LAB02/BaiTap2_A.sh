#! /bin/bash

src_dir=/home/trananhquoc-23521313/LAB02

echo "Nhap vao Ho Ten: "
read HoTen

echo "Tien Hanh Tao Thu Muc co ten: $HoTen"
mkdir "$src_dir/$HoTen"
if [ -d "$src_dir/$HoTen" ]
then 
    echo "Tao Thu Muc $HoTen Thanh Cong !"
else 
    echo "Tao Thuc Muc $HoTen That Bai !"
fi 

exit 0