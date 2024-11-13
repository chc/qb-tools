FILE=$1
qdeopt $FILE temp.d
qdump temp.d
rm temp.d
