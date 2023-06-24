# Простой скрипт для скачивания библиотеки Criterion
# Если уже существует директория - значит, ничего делать не нужно
if [ -d "./criterion" ]; then
    printf "Criterion was populated\n"
    exit 0
fi
# Создаем директорию populate
if [ ! -d "./populate" ]; then
    printf "Creating <populate> dir"
    mkdir populate
fi
cd populate &&
# Скачиваем архив релиза
wget https://github.com/Snaipe/Criterion/releases/download/v2.4.2/criterion-2.4.2-linux-x86_64.tar.xz &&
# Распаковываем
tar -xf criterion-2.4.2-linux-x86_64.tar.xz &&
# Переименовываем
mv criterion-2.4.2 criterion &&
# Переносим
mv criterion .. &&
cd ..  &&
# Чистим за собой директорию, в которую скачивали
rm -rf populate