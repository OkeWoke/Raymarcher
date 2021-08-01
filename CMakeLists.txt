cmake_minimum_required(VERSION 3.17)
project(Raymarcher)
find_package( OpenCV 3 REQUIRED )

set(CMAKE_CXX_STANDARD 14)
set(APP_ROOT .)
set(OBJECTS ./Objects)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_ROOT})
include_directories(${OBJECTS})
include_directories( ${OpenCV_INCLUDE_DIRS} )

#add_executable(Raymarcher main.cpp Vec.cpp Vec.h ${OBJECTS}/. )
add_executable(Raymarcher main.cpp Vec.cpp Vec.h ${OBJECTS}/IObject.h ${OBJECTS}/Sphere.cpp ${OBJECTS}/Sphere.h ${OBJECTS}/Plane.cpp ${OBJECTS}/Plane.h Objects/MandelBulb.cpp Objects/MandelBulb.h)
target_link_libraries(Raymarcher ${OpenCV_LIBS})



add_subdirectory("unit-tests")
