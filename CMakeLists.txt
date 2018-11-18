cmake_minimum_required( VERSION 2.8.12 )

project( utf_range_prj )

include( ExternalProject )

find_package( Boost 1.58.0 COMPONENTS system filesystem regex unit_test_framework iostreams REQUIRED )

add_definitions( -DBOOST_ALL_NO_LIB )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")
add_definitions( -DBOOST_TEST_DYN_LINK -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK )

include( "${CMAKE_SOURCE_DIR}/dependent_projects/CMakeListsCompiler.txt" )

set( DEP_ROOT_DIR_BASE "${CMAKE_SOURCE_DIR}/dependent_projects/build" )
if( "${CMAKE_BUILD_TYPE}" EQUAL "Debug" )
	set( DEP_ROOT_DIR "${DEP_ROOT_DIR_BASE}/debug" )
elseif( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
	set( DEP_ROOT_DIR "${DEP_ROOT_DIR_BASE}/release" )
endif( )

message( "Building for: ${CMAKE_BUILD_TYPE}" )
if( EXISTS "${DEP_ROOT_DIR}" )
	message( "using dependency folder" )
	add_custom_target( dependency_stub )
	include_directories( SYSTEM "${DEP_ROOT_DIR}/install/include" )
	link_directories( "${DEP_ROOT_DIR}/install/lib" )
else( )
	message( "downloading and building deps" )
	include( "${CMAKE_SOURCE_DIR}/dependent_projects/dependencies.txt" )
	add_library( dependency_stub STATIC "${CMAKE_SOURCE_DIR}/dependent_projects/stub.cpp" )
	add_dependencies( dependency_stub ${DEP_PROJECT_DEPS} )
endif( )

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
add_dependencies( utf_range dependency_stub )

add_library( utf_string STATIC ${HEADER_FOLDER}/daw/utf_range/daw_utf_string.h ${SOURCE_FOLDER}/daw_utf_string.cpp )
add_dependencies( utf_string dependency_stub )

enable_testing( )
add_custom_target( check COMMAND ${CMAKE_CTEST_COMMAND} )

install( TARGETS utf_range DESTINATION lib )
install( TARGETS utf_string DESTINATION lib )

install( DIRECTORY ${HEADER_FOLDER}/ DESTINATION include/ )
install( DIRECTORY third_party/include/ DESTINATION include )

add_executable( utf_range_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_range_test.cpp ${HEADER_FOLDER}/daw/utf_range/daw_utf_range.h )
target_link_libraries( utf_range_test_bin utf_range ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( utf_range_test_bin dependency_stub utf_range )
add_test( utf_range_test, utf_range_test_bin )
add_dependencies( check utf_range_test_bin )

add_executable( utf_string_test_bin EXCLUDE_FROM_ALL ${TEST_FOLDER}/daw_utf_string_test.cpp ${HEADER_FILES} )
target_link_libraries( utf_string_test_bin utf_range utf_string ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies( utf_string_test_bin dependency_stub utf_string )
add_test( utf_string_test, utf_string_test_bin )
add_dependencies( check utf_string_test_bin )

