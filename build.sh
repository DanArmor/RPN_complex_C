if [ ! -d "./build" ]; then
    mkdir build
fi

cd build &&
cmake .. &&
make

if [[ $? -ne 0 ]]; then
    printf "Error during building!\n"
    exit 1
fi