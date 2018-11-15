cmake_minimum_required( VERSION 2.8.12 )

project( utf_range_prj )

include( ExternalProject )

find_package( Boost 1.58.0 COMPONENTS system filesystem regex unit_test_framework iostreams REQUIRED )

add_definitions( -DBOOST_ALL_NO_LIB )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")
add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_definitions( -DNOMINMAX -DD_WIN32_WINNT=0x0601 )
else( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options( -march=native -pthread -Weverything -Wno-c++98-compat -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion -Wno-missing-braces)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3")
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-march=native -pthread -Wall -Wno-deprecated-declarations -Wduplicated-cond -Wlogical-op -Wnull-dereference -Wold-style-cast -Wshadow)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O3")
	endif( )
endif( )


externalproject_add(
    header_libraries_prj
    GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
#link_directories( "${CMAKE_BINARY_DIR}/install/lib" )

add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

set( HEADER_FILES
	${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h
	${HEADER_FOLDER}/daw/utf_range/daw_utf_string.h
)

include_directories( ${HEADER_FOLDER} )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
include_directories( "third_party/include" )
link_directories( ${Boost_LIBRARY_DIRS} )

add_library( utf_range STATIC ${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h ${SOURCE_FOLDER}/daw_utf_range.cpp )
add_dependencies( utf_range header_libraries_prj )

add_library( utf_string STATIC ${HEADER_FOLDER}/daw/utf_range/daw_utf_string.h ${SOURCE_FOLDER}/daw_utf_string.cpp )
add_dependencies( utf_string header_libraries_prj )

enable_testing( )
add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} )

install( TARGETS utf_range DESTINATION lib )
install( TARGETS utf_string DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/ )
install( DIRECTORY third_party/include/ DESTINATION include )

add_executable( utf_range_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_range_test.cpp ${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h )
target_link_libraries( utf_range_test_bin utf_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_test( utf_range_test, utf_range_test_bin )
add_dependencies( check utf_range_test_bin )

add_executable( utf_string_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_string_test.cpp ${HEADER_FILES} )
target_link_libraries( utf_string_test_bin utf_range utf_string ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_test( utf_string_test, utf_string_test_bin )
add_dependencies( check utf_string_test_bin )

