#!/bin/bash

# Đặt thư mục nguồn và đích
SOURCE_DIR=/home/trananhquoc-23521313/LAB02/OS_LAB2_IMG
DEST_DIR=/home/trananhquoc-23521313/LAB02/OS_LAB2_IMG/JPG

# Di chuyển tất cả các file JPG
mv "$SOURCE_DIR"/*.jpg "$DEST_DIR"

# Đếm số lượng file JPG đã di chuyển
count=$(ls -1 "$DEST_DIR"/*.jpg 2>/dev/null | wc -l)

# Xuất ra số lượng file JPG
echo "Số lượng ảnh PNG đã di chuyển: $count"
