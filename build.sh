# Скрипт для сборки проекта
# Создаем директорию для сборки
if [ ! -d "./build" ]; then
    mkdir build
fi

# Запускаем скачивание Criterion
./populate_criterion.sh &&
# Собираем проект
cd build &&
cmake .. &&
make

if [[ $? -ne 0 ]]; then
    printf "Error during building!\n"
    exit 1
fi