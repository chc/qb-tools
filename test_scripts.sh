#deopt all scripts
for file in $(find . -type f -name "*.qb" -print)
do
    echo $file;
    qdeopt $file "$file.d"
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
done