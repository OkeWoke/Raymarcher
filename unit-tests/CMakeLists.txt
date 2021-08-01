#set(CATCH_DIR "C:\\Program Files (x86)\\Catch2")
#find_package(Catch2 REQUIRED PATHS ${CATCH_DIR})
#add_executable(unit-tests "unit-tests.cpp" )
#target_link_libraries(unit-tests Catch2::Catch2)

#include(CTest)
#include(Catch)
#catch_discover_tests(unit-tests)
get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})

set(CMAKE_CXX_STANDARD 14)

find_package(Catch2 CONFIG REQUIRED)
add_executable(unit-tests unit-tests.cpp vec_tests.cpp ${PARENT_DIR}/Vec.h ${PARENT_DIR}/Vec.cpp )
target_link_libraries(unit-tests PRIVATE Catch2::Catch2)
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)