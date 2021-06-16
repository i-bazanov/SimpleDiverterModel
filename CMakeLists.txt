cmake_minimum_required(VERSION 3.0)

project(main)

set(ROOT_PATH ../)

add_subdirectory(${ROOT_PATH}model/lib/ lib/)

add_executable(${PROJECT_NAME} main.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

target_link_libraries(${PROJECT_NAME} PRIVATE lib)
