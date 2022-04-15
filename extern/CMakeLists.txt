# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/utf_range
#

include(FetchContent)

option(DAW_ENABLE_TESTING "Build unit tests" OFF)
set(DAW_ENABLE_TESTING OFF)

if (NOT TARGET daw::daw-header-libraries)
    FetchContent_Declare(
            daw-header-libraries
            GIT_REPOSITORY https://github.com/beached/header_libraries.git
    )
    FetchContent_MakeAvailable(daw-header-libraries)
endif ()

