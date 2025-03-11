cmake_minimum_required(VERSION 3.10)
project(go_quant_new)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")

# Find libwebsockets using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED libwebsockets)
# Include CURL
find_package(CURL REQUIRED)


add_definitions(-DSPDLOG_ENABLE_ASYNC_LOGGING)

# Find Boost (Ensure Boost is installed on your system)
find_package(Boost REQUIRED COMPONENTS date_time)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/libs/uWebSockets/src)
include_directories(${CMAKE_SOURCE_DIR}/libs/uSockets/src)
include_directories(${LWS_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/libs/spdlog/include)

# Link directories
link_directories(${LWS_LIBRARY_DIRS})

# Find all .cpp files in src/
file(GLOB SOURCES src/*.cpp)

# Add main executable
add_executable(go_quant_new ${SOURCES})

# Link Boost, uSockets, and required system libraries
target_link_libraries(go_quant_new 
    PRIVATE ${CMAKE_SOURCE_DIR}/libs/uSockets/uSockets.a 
    PRIVATE ${LWS_LIBRARIES} 
    PRIVATE ${Boost_LIBRARIES}
    PRIVATE crypto pthread ssl z ${CURL_LIBRARIES}
)



