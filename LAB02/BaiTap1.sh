#!/bin/bash

# Đường dẫn đến thư mục người dùng
user_dir=/home/trananhquoc-23521313/LAB02/OS_LAB2_IMG

# Kiểm tra thư mục PNG, nếu chưa tồn tại thì tạo
if [ ! -d "$user_dir/PNG" ]; then
  echo "Thư mục PNG chưa tồn tại. Đang tạo..."
  mkdir "$user_dir/PNG"
else
  echo "Thư mục PNG đã tồn tại."
fi

# Kiểm tra thư mục JPG, nếu chưa tồn tại thì tạo
if [ ! -d "$user_dir/JPG" ]; then
  echo "Thư mục JPG chưa tồn tại. Đang tạo..."
  mkdir "$user_dir/JPG"
else
  echo "Thư mục JPG đã tồn tại."
fi
