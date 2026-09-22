cmake_minimum_required(VERSION 3.28)
project(NetworkCore LANGUAGES CXX)

# The existing demo entry point remains the only compiled source.
# Networking sources still need the fixes listed in README.md before library extraction.
add_executable(NetworkCoreDemo src/main.cpp
                src/network/TcpConnection.cpp
                src/network/network.cpp
                src/network/TcpConnectionManager.cpp
)
target_include_directories(NetworkCoreDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

find_package(Boost 1.83 REQUIRED CONFIG)
target_link_libraries(NetworkCoreDemo PRIVATE Boost::headers)

# include(${CMAKE_CURRENT_LIST_DIR}/../cmake/GameDependencies.cmake)
# target_link_libraries(NetworkCoreDemo PRIVATE GameServer::Dependencies)
