#!/bin/bash

# Đường dẫn tới tệp monhoc.txt và thư mục đích (từ câu a)
FILE="/home/trananhquoc-23521313/LAB02/monhoc.txt"
DEST_DIR="/home/trananhquoc-23521313/LAB02/MonHoc"

# Đọc từng dòng từ tệp monhoc.txt
while IFS= read -r line; do
    # Lấy mã môn học (phần đầu tiên của dòng)
    ma_mon=$(echo "$line" | awk '{print $1}')

    # Tạo thư mục với tên mã môn học
    mkdir -p "$DEST_DIR/$ma_mon"
    
    echo "Đã tạo thư mục: $DEST_DIR/$ma_mon"
done < "$FILE"

exit 0