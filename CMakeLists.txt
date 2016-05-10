cmake_minimum_required( VERSION 2.8.0 )

project( char_range_prj )

find_package( Boost 1.59.0 COMPONENTS system filesystem regex unit_test_framework REQUIRED )
#set( Boost_Libs ${BOOST_Libraries} -lboost_system -lboost_regex -lboost_filesystem -lboost_program_options -lboost_unit_test_framework )
set( Boost_Libs ${Boost_LIBRARIES} )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601" )
	set( Boost_Libs )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -g -O3 -Weverything -Wno-c++98-compat  -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion -Wno-global-constructors" )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
	find_package( Threads REQUIRED )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O3 -Wall -Wno-deprecated-declarations" )
endif( )

set( HEADER_FOLDER "include" )

set( HEADER_FILES
	${HEADER_FOLDER}/daw_char_range.h
)

set( SOURCE_FILES
	daw_char_range.cpp
)

include_directories( ${HEADER_FOLDER} )
include_directories( SYSTEM "third_party/include" )
include_directories( SYSTEM ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

add_library( char_range STATIC ${HEADER_FILES} ${SOURCE_FILES} )

install( TARGETS char_range DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/daw/char_range )
install( DIRECTORY third_party/include/ DESTINATION include )
