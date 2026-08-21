cmake_minimum_required(VERSION 4.4)
project(CppCommandServer)
add_executable(CppCommandServer src/main.cpp
	src/t1.cpp
)
target_include_directories(CppCommandServer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/include
	)
