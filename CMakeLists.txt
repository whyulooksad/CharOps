cmake_minimum_required(VERSION 3.10)

project(CharOps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static") # -static 静态链接
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")       # -s 去掉符号表
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")                         # -O3 最优化
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")                     # -O3 最优化

# add_subdirectory(//wsl.localhost/Ubuntu-24.04/home/stw/CLI11)

include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.1.2
)
FetchContent_MakeAvailable(cli11)

add_executable(tools src/main.cpp)
#add_library(tools STATIC src/main.cpp)
target_link_libraries(tools CLI11::CLI11)