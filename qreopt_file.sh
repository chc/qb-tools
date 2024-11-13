FILE=$1
echo "Rebuilding file: $FILE"
qdeopt $FILE temp.d
rm $FILE
qopt temp.d $FILE
rm temp.d