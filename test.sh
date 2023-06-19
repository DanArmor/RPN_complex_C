./build.sh &&
for file in ./build/*_test
do
    "./$file"
    if [[ $? -ne 0 ]] ; then
        printf "Error during execution of tests!\n"
        exit 1
    fi
done