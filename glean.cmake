
include( ExternalProject )
externalproject_add(
  header_libraries_prj
  GIT_REPOSITORY "https://github.com/beached/header_libraries"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/dependencies/header_libraries"
  INSTALL_DIR "${CMAKE_BINARY_DIR}/install"
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install -DGLEAN_INSTALL_ROOT=${CMAKE_BINARY_DIR}/install
)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/install/include" )
link_directories( "${CMAKE_BINARY_DIR}/install/lib" )
set( DEP_PROJECT_DEPS header_libraries_prj )

