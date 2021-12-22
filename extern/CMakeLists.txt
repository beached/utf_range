# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/utf_range
#

include( FetchContent )

FetchContent_Declare(
	daw_header_libraries
	GIT_REPOSITORY https://github.com/beached/header_libraries.git
	GIT_TAG master
)

FetchContent_MakeAvailable(daw_header_libraries)

