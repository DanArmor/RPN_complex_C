# Тест для запуска тестов
# Соберем проект
./build.sh &&
# Выполним все файлы тестов
for file in ./build/*_test
do
    "./$file"
    if [[ $? -ne 0 ]] ; then
        printf "Error during execution of tests!\n"
        exit 1
    fi
done

printf "Tests are OK. Starting valgrind tests\n"

for file in ./build/*_test
do
    valgrind --leak-check=full --error-exitcode=1 "./$file"
    if [[ $? -ne 0 ]] ; then
        printf "Error during execution of valgrind tests!\n"
        exit 1
    fi
done