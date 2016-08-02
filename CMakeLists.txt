cmake_minimum_required( VERSION 2.8.12 )

project( char_range_prj )

find_package( Boost 1.58.0 REQUIRED )

IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL 'MSVC' )
	add_compile_options( -D_WIN32_WINNT=0x0601 ) 
ELSE( )
	IF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
		add_compile_options(-std=c++14 -stdlib=libc++ -Weverything -Wno-c++98-compat -Wfatal-errors -Wno-covered-switch-default -Wno-padded -Wno-exit-time-destructors -Wno-c++98-compat-pedantic -Wno-unused-parameter -Wno-missing-noreturn -Wno-missing-prototypes -Wno-disabled-macro-expansion)		
	ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
		add_compile_options(-std=c++14 -Wall -Wno-deprecated-declarations)
	ENDIF()
ENDIF()

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
