cmake_minimum_required(VERSION 3.16)

project(local_pc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "~/Qt/6.5.0/gcc_64") #modify this path to yours
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# add folder files
file(GLOB_RECURSE VISION_FILES src/*.cpp vision_analysis/src/*.hpp)
file(GLOB_RECURSE GUI_FILES gui/*.cpp gui/*.h gui/*.ui)

add_executable(${PROJECT_NAME}
    gui/resource/OwlResources.qrc
    src/local_pc.cpp
    ${GUI_FILES}
    ${VISION_FILES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Widgets
    ${OpenCV_LIBS}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
)
