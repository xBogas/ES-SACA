cmake_minimum_required(VERSION 3.0)
project( vision )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.4.3/gcc_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()


file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable( ${PROJECT_NAME} main.cpp
    ${SRC_FILES})


target_link_libraries( ${PROJECT_NAME}
	PRIVATE Qt6::Widgets
	${OpenCV_LIBS} 
	${QT_LIBRARIES})

project(align)
add_executable(align alignment.cpp)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
