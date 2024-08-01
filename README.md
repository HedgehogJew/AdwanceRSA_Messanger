# AdwancedRSA_Messanger
Implementation of the messenger with RSA and GOST28147 encryption, based on Boost ASIO.

Описание
Данный проект представляет собой реализацию мессенджера с использованием криптографических алгоритмов RSA и GOST28147 для обеспечения безопасной передачи сообщений. Мессенджер построен на основе библиотеки Boost ASIO для сетевого взаимодействия.
Возможности

    Шифрование сообщений с использованием алгоритма RSA
    Шифрование сообщений с использованием алгоритма ГОСТ-28147
    Многопоточная обработка входящих и исходящих сообщений

Установка и сборка
Зависимости

    Boost (включая Boost.Asio)
    CMake (для сборки)

Сборка

    Установите библиотеку Boost, включая Boost.Asio.
    Выполните следующие команды:

    bash
    cd /build
    cmake ..
    make

Использование
Запуск сервера
Для запуска сервера выполните:

bash

    ./build/bin/testServer

Запуск клиента
Для запуска клиента выполните:

bash

    ./build/bin/testClient

Технологии

    C++
    Boost ASIO
    RSA
    GOST28147
