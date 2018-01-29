project(PAPI_test)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)

# Set Cmake module path for find_package
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

find_package(PAPI)


add_executable(${PROJECT_NAME} ${SRC_LIST})


# Link target dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC
    PAPI
)
