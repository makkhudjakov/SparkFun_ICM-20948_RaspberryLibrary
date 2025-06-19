project(dmp_Quat9)

# Явное перечисление файлов примера
set(EXAMPLE_SOURCES
    main.cpp
)

add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ICM20948_Lib)