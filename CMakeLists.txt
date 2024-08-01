cmake_minimum_required(VERSION 3.10)

project(CryptoProject)

# Список исходных файлов
set(SOURCES
    src/AdvancedRSA/mathFuncs.cpp
    src/AdvancedRSA/stringAndSplit.cpp
    src/AdvancedRSA/RSAWizard.cpp
    src/AdvancedRSA/GOST28147.cpp
    src/Messenger/netMessage.cpp
    src/Messenger/connection.cpp
    src/Messenger/InputThreadCtrl.cpp
    src/Messenger/ConnectionThreadCtrl.cpp
    src/Messenger/OutputThreadCtrl.cpp
)

# Опции компилятора
add_compile_options(
    -Wall
)

# Библиотеки для связи
link_libraries(
    boost_system 
    boost_filesystem
)

# Каталоги для вывода
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_OBJECT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Цель исполняемого файла
add_executable(testServer src/Messenger/testServer.cpp ${SOURCES})

add_executable(testClient src/Messenger/client.cpp ${SOURCES})
