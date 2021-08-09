cmake_minimum_required(VERSION 3.17)
project(Raymarcher)
option(USE_OPENCV "Enables usage of opencv" ON)
if(USE_OPENCV)
    add_compile_definitions(USE_OPENCV)
    find_package( OpenCV 3 REQUIRED )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
endif()

set(CMAKE_CXX_STANDARD 14)
set(APP_ROOT .)
set(OBJECTS ./Objects)

find_package(Threads)
add_compile_options(-pthread)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_ROOT})
include_directories(${OBJECTS})
file(GLOB obj_src ${OBJECTS}/*.cpp)
file(GLOB cur_src ${APP_ROOT}/*.cpp)
add_executable(Raymarcher ${cur_src} ${obj_src})

if(USE_OPENCV)
    target_link_libraries(Raymarcher ${OpenCV_LIBS})
endif()
target_link_libraries(Raymarcher ${CMAKE_THREAD_LIBS_INIT})
add_subdirectory("unit-tests")
