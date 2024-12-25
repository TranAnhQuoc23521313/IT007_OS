#!/bin/bash

# Đặt thư mục nguồn và đích
SOURCE_DIR=/home/trananhquoc-23521313/LAB02/OS_LAB2_IMG
DEST_DIR=/home/trananhquoc-23521313/LAB02/OS_LAB2_IMG/PNG

# Di chuyển tất cả các file PNG
mv "$SOURCE_DIR"/*.png "$DEST_DIR"

# Đếm số lượng file PNG đã di chuyển
count=$(ls -1 "$DEST_DIR"/*.png 2>/dev/null | wc -l)

# Xuất ra số lượng file PNG
echo "Số lượng ảnh PNG đã di chuyển: $count"
