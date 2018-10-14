#!/bin/bash

function runAstyle {
    astyle --lineend=linux --suffix=none --style=break --attach-classes --convert-tabs \
    --indent-preproc-block --indent-preproc-cond --indent-classes --indent-cases \
    --indent-switches --indent-col1-comments --pad-oper --pad-comma --pad-header \
    --align-pointer=name --align-reference=name --add-braces --attach-return-type $1
}
export -f runAstyle

if [ $# -eq 0 ] || [ "$1" == "-h" ] || [ "$1" == "help" ]; then
    echo "Usage: apply-style.sh file.cpp file2.cpp ..."
    echo "       apply-style.sh *.cpp"
    echo "       apply-style.sh all"
    exit 0
fi

if [ "$1" == "all" ]; then
    find . \( -path './src/libs' -o -path './src/audio/OSXDriver' -o -path './build*' \) -prune \
    -o \( -name "*.cpp" -o -name "*.h" \) -exec bash -c 'runAstyle "$0"' {} \;
    exit $?
fi

for file in "$@"
do
    runAstyle $file
done