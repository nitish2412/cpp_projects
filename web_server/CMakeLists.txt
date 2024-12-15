cmake_minimum_required(VERSION 3.10)
project(HighPerformanceWebServer)

set(CMAKE_CXX_STANDARD 17)
add_executable(web_server src/main.cpp src/server.cpp )

#add_executable(web_server src/main.cpp src/server.cpp src/thread_pool.cpp)
