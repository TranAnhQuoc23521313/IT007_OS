#! /bin/sh

echo "Nhap vao ten:"
read name

echo "Nhap vao MSSV:"
read MSSV

if [ "$name" = "Tran Anh Quoc" ]
then
    if [ "$MSSV" = "23521313" ]
    then 
        echo "Chao mung $name quay lai !"
    else 
        echo "Access denied"
    fi
else 
    echo "Access denied"
fi

exit 0

#Running command: ~/LAB02/IdentifyStudent.sh

#C2: 
#    if [ "$name" = "Tran Anh Quoc" ] && [ "$MSSV" = "23521313" ]
#        ...
#C3:
#    if [ "$name" = "Tran Anh Quoc" -a "$MSSV" = "23521313"]