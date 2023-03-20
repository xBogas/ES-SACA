cmake_minimum_required(VERSION 3.16)

project(local_pc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/joao/Qt/6.4.3/gcc_64") #modify this path to yours
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(local_pc
    src/local_pc.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

target_link_libraries(local_pc
    PRIVATE Qt6::Widgets
    ${OpenCV_LIBS}
    )
