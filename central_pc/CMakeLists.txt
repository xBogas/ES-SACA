cmake_minimum_required(VERSION 3.16)

project(central_pc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "~/Qt/6.5.0/gcc_64; /usr/include/pqxx") # modify this path to yours
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

find_library(libpqxx_LIBRARY NAMES pqxx PATHS /usr/local/lib /usr/lib)

# add folder files
file(GLOB_RECURSE DATABASE_FILES database/*.cpp database/*.h)
file(GLOB_RECURSE GUI_FILES gui/*.cpp gui/*.h gui/*.ui)

add_executable(${PROJECT_NAME}
    gui/resource/OwlResources.qrc
    src/central_pc.cpp
    ${DATABASE_FILES}
    ${GUI_FILES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Widgets
    ${OpenCV_LIBS}
    ${QT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${libpqxx_LIBRARY}
    # ${Vulkan_LIBRARIES}
)
