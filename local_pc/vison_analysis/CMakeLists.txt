cmake_minimum_required(VERSION 3.0)
project( align )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ${PROJECT_NAME} alignment.cpp )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
