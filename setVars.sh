#!/bin/bash

PWD=`pwd`
for file in `find $PWD -maxdepth 2 -name "*.vars"`
    do
    echo "reading "$file" ..."
    for l in `cat $file`
        do
        echo "     setting up ... "$l
        eval "export" $l
        done
    done

