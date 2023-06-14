cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "~/Qt/6.5.0/gcc_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(ESP_COMS	"Esp communication"	OFF)
option(TESTING	"Testing"			ON)
option(DEBUG ON)

project( vision )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

if(ESP_COMS)
message("Compiling with flag ESP_COMS")
	add_definitions(-DESP_COMS)
endif(ESP_COMS)

if(TESTING)
	message("Compiling with flag VISION_TEST")
	add_definitions(-DVISION_TEST)
endif(TESTING)

if(TESTING)
	message("Compiling with flag DEBUG")
	add_definitions(-DDEBUG)
endif(TESTING)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()


file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable( ${PROJECT_NAME} main.cpp
    ${SRC_FILES})


target_link_libraries( ${PROJECT_NAME}
	PRIVATE Qt6::Widgets
	${OpenCV_LIBS} 
	${QT_LIBRARIES})
