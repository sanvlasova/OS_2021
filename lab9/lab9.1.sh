#!/bin/bash

g++ -c lab9.1.cpp
g++ -o lab9.1 lab9.1.o -lacl

echo "@ Создаем файл cat-says"
echo "meow" > cat-says

echo "---"
echo "@ Узнаем о файле информацию с помощью команды getfacl"
getfacl ./cat-says

echo "---"
echo "@ Убираем возможность записи для пользователей с помощью команды setfacl"
setfacl -m u::r ./cat-says

echo "---"
echo "@ Узнаем о файле информацию с помощью команды getfacl"
getfacl ./cat-says

echo "---"
echo "@ Пытаемся записать в файл"
echo "meow meow" > cat-says

echo "---"
echo "@ Запускаем программу"
./lab9.1

echo "---"
echo "@ Узнаем о файле информацию с помощью команды getfacl"
getfacl ./cat-says

echo "---"
echo "@ Пытаемся записать в файл"
echo "meow meow" > cat-says

echo "---"
echo "@ Подчищаем за собой"
unlink cat-says

