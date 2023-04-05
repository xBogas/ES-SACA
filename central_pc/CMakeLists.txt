cmake_minimum_required(VERSION 3.16)

project(central_pc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "~/Qt/6.4.3/gcc_64") #modify this path to yours
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(central_pc
    gui/resource/OwlResources.qrc
    src/central_pc.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/mainwindow.ui
    gui/pistolwindow.cpp
    gui/pistolwindow.h
    gui/pistolwindow.ui
    gui/loginwindow.cpp
    gui/loginwindow.h
    gui/loginwindow.ui
)

target_link_libraries(central_pc
    PRIVATE Qt6::Widgets
    ${OpenCV_LIBS}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
)
