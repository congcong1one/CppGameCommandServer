cmake_minimum_required(VERSION 3.28)
project(LoginServer LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GameDependencies.cmake)

# Keep the executable name expected by the existing VS Code launch configuration.
add_executable(CppCommandServer src/main.cpp)
target_link_libraries(CppCommandServer PRIVATE GameServer::Dependencies)
