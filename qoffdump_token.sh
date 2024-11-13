for file in $(find . -type f -name "*.qb" -print)
do
    echo $file;
    qoffdump_token $file
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
done