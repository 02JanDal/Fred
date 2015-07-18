cmake_minimum_required(VERSION 3.0)

find_package(Qt5Core REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(PkgConfig REQUIRED)

include(ExternalProject)

ExternalProject_ADD(qmsgpack_src GIT_REPOSITORY git@github.com:romixlab/qmsgpack.git CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)

add_library(qmsgpack SHARED IMPORTED)
add_dependencies(qmsgpack qmsgpack_src)
set_property(TARGET qmsgpack APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(qmsgpack PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "Qt5::Core"
  IMPORTED_LOCATION_NOCONFIG "${CMAKE_BINARY_DIR}/qmsgpack_src-prefix/lib/libqmsgpack.so.0.1.0"
  IMPORTED_SONAME_NOCONFIG "libqmsgpack.so.0"
)
set(QMSGPACK_SRC_DIR "${CMAKE_BINARY_DIR}/qmsgpack_src-prefix/src/qmsgpack_src")

pkg_search_module(LIBBSON REQUIRED libbson-1.0)
pkg_search_module(MSGPACK REQUIRED msgpack)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${RAPIDJSON_INCLUDE_DIRS} ${LIBBSON_INCLUDE_DIRS} ${MSGPACK_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/qmsgpack_src-prefix/include)

configure_file(${CMAKE_SOURCE_DIR}/environment.in ${CMAKE_BINARY_DIR}/environment @ONLY)

function(add_tester name)
	add_executable(${name} ${name}.cpp)
	target_link_libraries(${name} ${ARGN})
endfunction()

add_tester(json_cpp_qt Qt5::Core)
add_tester(json_cpp_rapidjson)
add_tester(bson_cpp_libbson ${LIBBSON_LIBRARIES})
add_tester(msgpack_cpp_msgpack ${MSGPACK_LIBRARIES})
add_tester(msgpack_cpp_qmsgpack Qt5::Core qmsgpack)
