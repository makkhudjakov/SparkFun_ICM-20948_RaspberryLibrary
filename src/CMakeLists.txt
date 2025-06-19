# Create a static library from sources
set(LIB_NAME ICM20948_Lib)

# Основные исходные файлы библиотеки
set(LIB_SOURCES
   ICM_20948.cpp
   ICM_20948.h
   I2C.h
)

# Файлы из папки util
set(UTIL_SOURCES
   util/AK09916_ENUMERATIONS.h 
   util/AK09916_REGISTERS.h
   util/ICM_20948_C.c
   util/ICM_20948_C.h
   util/ICM_20948_DMP.h
   util/ICM_20948_ENUMERATIONS.h
   util/ICM_20948_REGISTERS.h
   util/icm20948_img.dmp3a.h
)

# Объединяем все исходники
set(ALL_SOURCES ${LIB_SOURCES} ${UTIL_SOURCES})

# Исключаем файлы из поддиректорий если нужно (опционально)
list(FILTER ALL_SOURCES EXCLUDE REGEX ".*/tests?/.*")

# Создаем библиотеку
add_library(${LIB_NAME} STATIC ${ALL_SOURCES})

# Включаем директории
target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/util>
    $<INSTALL_INTERFACE:include>
)

# Публичные зависимости (если есть)
# target_link_libraries(${LIB_NAME} PUBLIC some_dependency)

# Установка библиотеки
install(TARGETS ${LIB_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Установка заголовков
install(DIRECTORY . DESTINATION include
    FILES_MATCHING 
    PATTERN "*.hpp" 
    PATTERN "*.h"
    PATTERN "util" EXCLUDE  # Если нужно исключить util из публичных заголовков
    PATTERN "CMakeLists.txt" EXCLUDE
)  