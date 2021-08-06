
get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})
include_directories(${PARENT_DIR}/Objects)

set(CMAKE_CXX_STANDARD 14)
find_package(Catch2 CONFIG REQUIRED)

file(GLOB test_src ${PARENT_DIR}/unit-tests/*.cpp)
file(GLOB obj_src ${PARENT_DIR}/Objects/*.cpp) #Objects src files
file(GLOB par_src ${PARENT_DIR}/*.cpp)
list (REMOVE_ITEM par_src ${PARENT_DIR}/main.cpp)
add_executable(unit-tests ${test_src} ${par_src} ${obj_src})

link_directories(${PARENT_DIR}/Objects/)
target_link_libraries(unit-tests PRIVATE Catch2::Catch2)
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)