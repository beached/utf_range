cmake_minimum_required(VERSION 3.10)
project( utf_range_prj )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

enable_testing()

include("${CMAKE_SOURCE_DIR}/dependent_projects/CMakeListsCompiler.txt")

include_directories("./include")
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GLEAN_CACHE "${CMAKE_SOURCE_DIR}/.glean/debug")
    set(GLEAN_CACHE2 "${CMAKE_SOURCE_DIR}/.glean/release")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(GLEAN_CACHE "${CMAKE_SOURCE_DIR}/.glean/release")
    set(GLEAN_CACHE2 "${CMAKE_SOURCE_DIR}/.glean/debug")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(GLEAN_CACHE "${CMAKE_SOURCE_DIR}/.glean/release")
    set(GLEAN_CACHE2 "${CMAKE_SOURCE_DIR}/.glean/debug")
endif ()

message("Building for: ${CMAKE_BUILD_TYPE}")
message("Checking for glean cache: '${GLEAN_CACHE}'")

option(GLEAN_INSTALL_ROOT "")
if (EXISTS "${GLEAN_INSTALL_ROOT}")
    message("using glean dependencies")
    message("installing to ${GLEAN_INSTALL_ROOT}")

    add_custom_target(dependency_stub)
    include_directories(SYSTEM "${GLEAN_INSTALL_ROOT}/include")
    link_directories("${GLEAN_INSTALL_ROOT}/lib")
elseif (EXISTS "${GLEAN_CACHE}")
    message("using glean dependencies")
    add_custom_target(dependency_stub)
    include_directories(SYSTEM "${GLEAN_CACHE}/include")
    link_directories("${GLEAN_CACHE}/lib")
elseif (EXISTS "${GLEAN_CACHE2}")
    message("looks like glean is in use but missing ${GLEAN_CACHE}")
elseif (EXISTS "${CMAKE_SOURCE_DIR}/glean.cmake")
    message("glean.cmake detected")
    include("${CMAKE_SOURCE_DIR}/glean.cmake")
    add_library(dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp")
    add_dependencies(dependency_stub ${DEP_PROJECT_DEPS})
    include_directories(SYSTEM "${CMAKE_BINARY_DIR}/install/include")
    link_directories("${CMAKE_BINARY_DIR}/install/lib")
endif ()

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

set( HEADER_FILES
	${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h
	${HEADER_FOLDER}/daw/utf_range/daw_utf_string.h
)

include_directories( ${HEADER_FOLDER} )

add_library( utf_range ${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h ${SOURCE_FOLDER}/daw_utf_range.cpp )
add_dependencies( utf_range dependency_stub )

add_library( utf_string ${HEADER_FOLDER}/daw/utf_range/daw_utf_string.h ${SOURCE_FOLDER}/daw_utf_string.cpp )
add_dependencies( utf_string dependency_stub )
target_link_libraries( utf_string utf_range )

enable_testing( )
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_custom_target(full)

install( TARGETS utf_range DESTINATION lib )
install( TARGETS utf_string DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/ )

add_executable( utf_range_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_range_test.cpp ${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h )
target_link_libraries( utf_range_test_bin utf_range ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( utf_range_test_bin dependency_stub utf_range )
add_test( NAME utf_range_test COMMAND utf_range_test_bin )
add_dependencies( check utf_range_test_bin )
add_dependencies( full utf_range_test_bin )

add_executable( utf_string_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_string_test.cpp ${HEADER_FILES} )
target_link_libraries( utf_string_test_bin utf_range utf_string ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( utf_string_test_bin dependency_stub utf_string )
add_test( NAME utf_string_test COMMAND utf_string_test_bin )
add_dependencies( check utf_string_test_bin )
add_dependencies( full utf_string_test_bin )

