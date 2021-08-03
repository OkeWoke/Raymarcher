
get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})
include_directories(${PARENT_DIR}/Objects)

set(CMAKE_CXX_STANDARD 14)
find_package(Catch2 CONFIG REQUIRED)

file(GLOB obj_src ${PARENT_DIR}/Objects/*.cpp) #Objects src files

add_executable(unit-tests unit-tests.cpp vec_tests.cpp ${PARENT_DIR}/Vec.h ${PARENT_DIR}/Vec.cpp OBJ_tests.cpp ${obj_src})
link_directories(${PARENT_DIR}/Objects/)
target_link_libraries(unit-tests PRIVATE Catch2::Catch2)
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)