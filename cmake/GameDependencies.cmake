include_guard(GLOBAL)

# The checked-in client bundle is built for Ubuntu 24.04 / x86_64.
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
   NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$" OR CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "The bundled clients require Ubuntu 24.04 x86_64 and a native build.")
endif()

get_filename_component(GAME_DEPENDENCIES_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../third_party/ubuntu24.04-x86_64" ABSOLUTE)
set(_game_include "${GAME_DEPENDENCIES_ROOT}/include")
set(_game_lib "${GAME_DEPENDENCIES_ROOT}/lib")

foreach(_file IN ITEMS lib/libmysqlcppconn.so lib/libmysqlcppconnx.so
        lib/libredis++.so lib/libhiredis.so lib/libprotobuf.so
        lib/libprotobuf-lite.so lib/libprotoc.so bin/protoc
        include/mysql/jdbc.h include/sw/redis++/redis++.h
        include/hiredis/hiredis.h include/google/protobuf/message.h)
    if(NOT EXISTS "${GAME_DEPENDENCIES_ROOT}/${_file}")
        message(FATAL_ERROR "Missing bundled dependency: ${_file}. See third_party/README.md.")
    endif()
endforeach()

add_library(mysql::concpp-jdbc SHARED IMPORTED GLOBAL)
set_target_properties(mysql::concpp-jdbc PROPERTIES
    IMPORTED_LOCATION "${_game_lib}/libmysqlcppconn.so"
    INTERFACE_INCLUDE_DIRECTORIES "${_game_include};${_game_include}/jdbc")

add_library(mysql::concpp SHARED IMPORTED GLOBAL)
set_target_properties(mysql::concpp PROPERTIES
    IMPORTED_LOCATION "${_game_lib}/libmysqlcppconnx.so"
    INTERFACE_INCLUDE_DIRECTORIES "${_game_include}")

add_library(hiredis::hiredis SHARED IMPORTED GLOBAL)
set_target_properties(hiredis::hiredis PROPERTIES
    IMPORTED_LOCATION "${_game_lib}/libhiredis.so"
    INTERFACE_INCLUDE_DIRECTORIES "${_game_include}")

find_package(Threads REQUIRED)
add_library(redis++::redis++ SHARED IMPORTED GLOBAL)
set_target_properties(redis++::redis++ PROPERTIES
    IMPORTED_LOCATION "${_game_lib}/libredis++.so"
    INTERFACE_INCLUDE_DIRECTORIES "${_game_include}"
    INTERFACE_LINK_LIBRARIES "hiredis::hiredis;Threads::Threads")

# Pin both the compiler and the runtime to the bundle, even when the host has
# another Protobuf version installed. FindProtobuf also provides protobuf_generate.
set(Protobuf_INCLUDE_DIR "${_game_include}")
set(Protobuf_LIBRARY "${_game_lib}/libprotobuf.so")
set(Protobuf_LIBRARY_RELEASE "${Protobuf_LIBRARY}")
set(Protobuf_LIBRARY_DEBUG "${Protobuf_LIBRARY}")
set(Protobuf_LITE_LIBRARY "${_game_lib}/libprotobuf-lite.so")
set(Protobuf_LITE_LIBRARY_RELEASE "${Protobuf_LITE_LIBRARY}")
set(Protobuf_LITE_LIBRARY_DEBUG "${Protobuf_LITE_LIBRARY}")
set(Protobuf_PROTOC_LIBRARY "${_game_lib}/libprotoc.so")
set(Protobuf_PROTOC_LIBRARY_RELEASE "${Protobuf_PROTOC_LIBRARY}")
set(Protobuf_PROTOC_LIBRARY_DEBUG "${Protobuf_PROTOC_LIBRARY}")
set(Protobuf_PROTOC_EXECUTABLE "${GAME_DEPENDENCIES_ROOT}/bin/protoc")
find_package(Protobuf 3.21.12 EXACT MODULE REQUIRED)

add_library(game_dependencies INTERFACE)
add_library(GameServer::Dependencies ALIAS game_dependencies)
target_compile_features(game_dependencies INTERFACE cxx_std_17)
target_link_libraries(game_dependencies INTERFACE
    mysql::concpp-jdbc redis++::redis++ protobuf::libprotobuf Threads::Threads)
