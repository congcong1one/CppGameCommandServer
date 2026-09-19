cmake_minimum_required(VERSION 3.28)
project(CppCommandServer)
add_executable(CppCommandServer src/main.cpp

)
target_include_directories(CppCommandServer
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/include
	${CMAKE_CURRENT_SOURCE_DIR}/src/network
	${CMAKE_CURRENT_SOURCE_DIR}/src/Session
	${CMAKE_CURRENT_SOURCE_DIR}/src/player/Player
	)

find_package(Boost 1.83 REQUIRED CONFIG)
target_link_libraries(CppCommandServer PRIVATE Boost::headers)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GameDependencies.cmake)
target_link_libraries(CppCommandServer PRIVATE GameServer::Dependencies)
