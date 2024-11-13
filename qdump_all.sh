for file in $(find . -type f -name "*.qb.d" -print)
do
    echo $file;
    OUT_NAME="$file.q"
    if [ -f $OUT_NAME ]; then
        continue
    fi
    qdump $file > $OUT_NAME
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
done