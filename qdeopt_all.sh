for file in $(find . -type f -name "*.qb" -print)
do
    echo $file;
    if [ -f "$file.d" ]; then
        continue
    fi
    qdeopt $file "$file.d"
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
done