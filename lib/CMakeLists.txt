cmake_minimum_required(VERSION 3.14)
project(qmlext CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml)

set(CMAKE_AUTOMOC ON)

add_library(${PROJECT_NAME} STATIC
    include/qmlext/eventbus.h
    include/qmlext/item.h
    include/qmlext/listmodel.h
    include/qmlext/event/event.h
    include/qmlext/event/eventprocessor.h
    include/qmlext/event/json/jsoneventadapter.h
    include/qmlext/event/json/jsoneventprocessor.h

    src/eventbus.cpp
    src/item.cpp
    src/listmodel.cpp
    src/event/event.cpp
    src/event/eventprocessor.cpp
    src/event/json/jsoneventadapter.cpp
    src/event/json/jsoneventprocessor.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} Qt5::Qml)

add_subdirectory(test)