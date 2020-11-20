cmake_minimum_required(VERSION 3.10)
include(FindPackageHandleStandardArgs)
include(GNUInstallDirs)

project(TSP LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB TSP_SRC
  "*.cpp"
  "*.h"
  "*.hpp"
)

add_executable(TSP ${TSP_SRC})
