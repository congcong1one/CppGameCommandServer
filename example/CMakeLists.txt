cmake_minimum_required(VERSION 3.28)
project(CppCommandServer)
add_executable(CppCommandServer src/main.cpp

)
target_include_directories(CppCommandServer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/include
	/src/network/network.cpp
	)

find_package(Boost 1.83 REQUIRED CONFIG)
target_link_libraries(CppCommandServer PRIVATE Boost::headers)
