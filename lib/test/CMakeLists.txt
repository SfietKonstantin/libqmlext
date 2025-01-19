cmake_minimum_required(VERSION 3.14)
project(qmlext-test CXX)
enable_testing(true)

find_package(Qt5 REQUIRED COMPONENTS Test)

set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME}
    tst_qmlext.cpp
)
add_test(${PROJECT_NAME} ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE qmlext Qt5::Test)
