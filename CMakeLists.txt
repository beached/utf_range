cmake_minimum_required( VERSION 2.8.12 )

project( char_range_prj )

find_package( Boost 1.58.0 COMPONENTS system filesystem regex unit_test_framework iostreams REQUIRED )

add_definitions( -DBOOST_TEST_DYN_LINK ) 
IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
ELSE( )
	if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" )
		add_compile_options(-std=c++14 -Weverything -Wno-c++98-compat -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)		
	ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -Wall -Wno-deprecated-declarations)
	ENDIF()
ENDIF()

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
include_directories( SYSTEM "third_party/include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

add_library( char_range STATIC ${HEADER_FILES} ${SOURCE_FILES} )

install( TARGETS char_range DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/char_range )
install( DIRECTORY third_party/include/ DESTINATION include )

add_executable( char_range_test_bin ${TEST_FOLDER}/daw_char_range_test.cpp ${HEADER_FILES} )
target_link_libraries( char_range_test_bin char_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_test( char_range_test, char_range_test_bin )

