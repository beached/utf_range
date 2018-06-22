cmake_minimum_required( VERSION 2.8.12 )

project( char_range_prj )

include( ExternalProject )

find_package( Boost 1.58.0 COMPONENTS system filesystem regex unit_test_framework iostreams REQUIRED )

add_definitions( -DBOOST_ALL_NO_LIB )

IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 /std:c++latest ) 
ELSE( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options(-std=c++14 -pthread -Weverything -Wno-c++98-compat -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -pthread -Wall -Wno-deprecated-declarations)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	endif( )
ENDIF()

externalproject_add(
    header_libraries_prj
    GIT_REPOSITORY "https://github.com/beached/header_libraries.git"
    SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
    GIT_TAG "master"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )

set( HEADER_FOLDER "include" )
set( SOURCE_FOLDER "src" )
set( TEST_FOLDER "tests" )

set( HEADER_FILES
	${HEADER_FOLDER}/daw_char_range.h
)

set( SOURCE_FILES
	${SOURCE_FOLDER}/daw_char_range.cpp
)

include_directories( ${HEADER_FOLDER} )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
include_directories( "third_party/include" )
link_directories( ${Boost_LIBRARY_DIRS} )

add_library( char_range STATIC ${HEADER_FILES} ${SOURCE_FILES} )
add_dependencies( char_range header_libraries_prj )

enable_testing( )
add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} )

install( TARGETS char_range DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/char_range )
install( DIRECTORY third_party/include/ DESTINATION include )

add_executable( char_range_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_char_range_test.cpp ${HEADER_FILES} )
target_link_libraries( char_range_test_bin char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

add_test( char_range_test, char_range_test_bin )
add_dependencies( check char_range_test_bin )

