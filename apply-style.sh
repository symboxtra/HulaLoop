#!/bin/bash

if [ $# -eq 0 ] || [ "$1" == "-h" ] || [ "$1" == "help" ]; then
    echo "Usage: apply-style.sh file.cpp file2.cpp ..."
    echo "       apply-style.sh *.cpp"
    exit 0
fi

for file in "$@"
do
    astyle --lineend=linux --suffix=none --style=break --attach-classes --convert-tabs --indent-classes --indent-cases --indent-switches --indent-col1-comments --pad-oper --pad-comma --pad-header --align-pointer=name --align-reference=name --add-braces --attach-return-type $file
done
