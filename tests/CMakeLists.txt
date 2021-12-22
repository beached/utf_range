#Copyright( c ) Darrell Wright
#
#Distributed under the Boost Software License, Version 1.0.( See accompanying
#file LICENSE or copy at http: // www.boost.org/LICENSE_1_0.txt)
#
#Official repository : https: // github.com/beached/header_libraries
#

#Allows building all in some IDE's
add_custom_target(daw-utf_range_full)

add_library(daw_utf_range_test_lib INTERFACE)
target_link_libraries(daw_utf_range_test_lib INTERFACE daw::daw-header-libraries daw::daw-utf-range ${COMPILER_SPECIFIC_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_options(daw_utf_range_test_lib INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/permissive->)


add_executable(daw_utf_range daw_utf_range_test.cpp)
target_link_libraries(daw_utf_range PRIVATE daw_utf_range_test_lib)
add_test(NAME daw_utf_range_test COMMAND daw_utf_range)
add_dependencies(daw-utf_range_full daw_utf_range)

add_executable(daw_utf_string daw_utf_string_test.cpp)
target_link_libraries(daw_utf_string PRIVATE daw_utf_range_test_lib)
add_test(NAME daw_utf_string_test COMMAND daw_utf_range)
add_dependencies(daw-utf_range_full daw_utf_string)
