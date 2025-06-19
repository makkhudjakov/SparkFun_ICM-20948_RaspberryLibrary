cmake_minimum_required(VERSION 3.17)
project(ICM20948 VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)  
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Глобальные настройки проекта
add_subdirectory(src)          # Основная библиотека
add_subdirectory(examples)     # Примеры использования