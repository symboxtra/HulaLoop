# Check args, if there is not any then just default to build path
if [ $# -eq 0 ]; then
    pathToExec="../build/bin/hulaloop-cli"
else
    pathToExec=$1
fi
# Run help2man
help2man $pathToExec > hulaloop.1
# Check errors in running help2man
if [ $? -eq 1 ]; then
    echo "Error running help2man exiting..."
    exit 1
fi
# Gzip because that is the format of the man page
gzip hulaloop.1
# Move the file to the man directory
sudo cp hulaloop.1.gz /usr/share/man/man1
# Cleanup
rm hulaloop.1.gz
