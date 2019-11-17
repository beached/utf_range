cmake_minimum_required( VERSION 2.8.12 )

project( dependency_prj )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested.")

include( "CMakeListsCompiler.txt" )

include_directories( "./include" )

include( "dependencies.txt" )

add_library( dependency_stub STATIC stub.cpp )
add_dependencies( dependency_stub ${DEP_PROJECT_DEPS} )

