cmake_minimum_required(VERSION 3.0)
project( Example )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( main.exe src/main.cpp )
target_link_libraries( main.exe ${OpenCV_LIBS} )