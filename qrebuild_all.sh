for file in $(find . -type f -name "*.qb" -print)
do
    echo "Processing file: $file";
    DEOPT_NAME="$file.d"
    OPT_NAME="$file.o"

    qdeopt $file $DEOPT_NAME
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
    qopt $DEOPT_NAME $OPT_NAME
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
    
    rm $file $DEOPT_NAME
    mv $OPT_NAME $file
    echo "DONE Processing file: $file";



done

