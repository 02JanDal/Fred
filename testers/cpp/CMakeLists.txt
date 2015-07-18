cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(CMakeParseArguments)
function(add_tester)
  set(options )
  set(oneValueArgs TARGET)
  set(multiValueArgs LIBRARIES SRCS INCLUDES DEFINITIONS)
  cmake_parse_arguments(AT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if("${AT_SRCS}" STREQUAL "")
    set(AT_SRCS ${AT_TARGET}.cpp)
  endif()

  add_executable(${AT_TARGET} "${AT_SRCS}")
  if(NOT "${AT_LIBRARIES}" STREQUAL "")
    target_link_libraries(${AT_TARGET} "${AT_LIBRARIES}")
  endif()
  if(NOT "${AT_INCLUDES}" STREQUAL "")
    target_include_directories(${AT_TARGET} PRIVATE "${AT_INCLUDES}")
  endif()
  if(NOT "${AT_DEFINITIONS}" STREQUAL "")
    target_compile_definitions(${AT_TARGET} PRIVATE "${AT_DEFINITIONS}")
  endif()
endfunction()

###################### FlatBuffers ######################
find_package(FlatBuffers)
if(FLATBUFFERS_FOUND)
  flatbuffers_generate_c_headers(FB ${CMAKE_BINARY_DIR}/flatbuffers.fbs)
  add_tester(TARGET flatbuffers_cpp_flatbuffers
    SRCS flatbuffers_cpp_flatbuffers.cpp ${FB_OUTPUTS}
  )
endif()

###################### ProtoBuf ######################
set(PROTOBUF_IMPORT_DIRS ${CMAKE_BINARY_DIR})
find_package(Protobuf)
if(PROTOBUF_FOUND)
  protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${CMAKE_BINARY_DIR}/protobuf.proto)
  add_tester(TARGET protobuf_cpp_protobuf
    INCLUDES ${PROTOBUF_INCLUDE_DIRS}
    LIBRARIES ${PROTOBUF_LIBRARY}
    SRCS protobuf_cpp_protobuf.cpp ${PROTO_SRC} ${PROTO_HDR}
  )
endif()

###################### QMsgPack & QJson* ######################
find_package(Qt5Core)
if(Qt5Core_FOUND)
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

  add_tester(TARGET json_cpp_qt LIBRARIES Qt5::Core)
  add_tester(TARGET msgpack_cpp_qmsgpack LIBRARIES Qt5::Core qmsgpack INCLUDES ${CMAKE_BINARY_DIR}/qmsgpack_src-prefix/include)
endif()

###################### RapidJSON ######################
find_package(RapidJSON)
if(RapidJSON_FOUND)
  add_tester(TARGET json_cpp_rapidjson INCLUDES ${RAPIDJSON_INCLUDE_DIRS})
endif()

find_package(PkgConfig REQUIRED)

###################### libbson ######################
pkg_search_module(LIBBSON libbson-1.0)
if(LIBBSON_FOUND)
  add_tester(TARGET bson_cpp_libbson LIBRARIES ${LIBBSON_LIBRARIES} INCLUDES ${LIBBSON_INCLUDE_DIRS})
endif()

###################### msgpack-c ######################
pkg_search_module(MSGPACK msgpack)
if(MSGPACK_FOUND)
  add_tester(TARGET msgpack_cpp_msgpack LIBRARIES ${MSGPACK_LIBRARIES} INCLUDES ${MSGPACK_INCLUDE_DIRS})
endif()

###################### Cap'n Proto ######################
find_package(CapnProto)
if(CAPNP_FOUND)
  set(CAPNPC_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
  set(CAPNPC_SRC_PREFIX "${CMAKE_BINARY_DIR}")

  capnp_generate_cpp(CAPNP_SRC CAPNP_HDR ${CMAKE_BINARY_DIR}/capnproto.capnp)

  add_tester(TARGET capnp_cpp_capnp
    SRCS capnp_cpp_capnp.cpp ${CAPNP_SRC} ${CAPNP_HDR}
    LIBRARIES ${CAPNP_LIBRARIES_LITE}
    INCLUDES ${CAPNP_INCLUDE_DIRS} ${CAPNPC_OUTPUT_DIR}
    DEFINITIONS ${CAPNP_DEFINITIONS}
  )
endif()

configure_file(${CMAKE_SOURCE_DIR}/environment.in ${CMAKE_BINARY_DIR}/environment @ONLY)
