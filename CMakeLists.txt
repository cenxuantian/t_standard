cmake_minimum_required(VERSION 3.0.0)
project(t_standard)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/t_standard)

add_executable(t_standard main.cpp)

if(WIN32)
if(MINGW)
target_link_libraries(t_standard atomic)
endif()
elseif(UNIX)
target_link_libraries(tmc pthread)
endif()