for file in $(find . -type f -name "*.qb.d.qb" -print)
do
    echo $file;
    rm $file
done

for file in $(find . -type f -name "*.qb.d" -print)
do
    echo $file;
    qopt $file "$file.qb"
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        exit 1
    fi
done


#re-qdeopt it
for file in $(find . -type f -name "*.qb.d.qb" -print)
do
    echo $file;
    qdeopt $file "$file.d"
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        exit 1
    fi
done