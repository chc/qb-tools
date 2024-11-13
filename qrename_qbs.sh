for file in $(find . -type f -name "*.qb.ps3" -print)
do
    nlen=${#file}
    newname=${file::nlen-4}
    #echo $newname
    mv $file $newname
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
done
