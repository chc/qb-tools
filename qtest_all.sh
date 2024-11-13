#decompile all
#for file in $(find . -type f -name "*.d" -print)
    #rm $file;
#done

#deopt all
for file in $(find . -type f -name "*.qb" -print)
do
    echo "Processing file: $file";
    DEOPT_NAME="$file.d"
    OPT_NAME="$file.o"
    DEOPT2_NAME="$file.t"

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
    qdeopt $OPT_NAME $DEOPT2_NAME 
    STATUS=$?
    if [ $STATUS -ne 0 ]; then
        break
    fi
    
    rm $DEOPT_NAME $OPT_NAME $DEOPT2_NAME;
    echo "DONE Processing file: $file";



done

