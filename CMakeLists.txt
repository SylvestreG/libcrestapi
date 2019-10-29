cmake_minimum_required(VERSION 3.14)
project(libcrestapi CXX)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

ExternalProject_Add(restclient-cpp
  GIT_REPOSITORY https://github.com/mrtazz/restclient-cpp/
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

ExternalProject_Get_Property(restclient-cpp source_dir)
ExternalProject_Get_Property(restclient-cpp binary_dir)

include_directories(${source_dir}/include)
include_directories(${binary_dir}/include)
link_directories(${binary_dir})

ExternalProject_Add(json11_lib
  GIT_REPOSITORY https://github.com/dropbox/json11/
  TIMEOUT 10
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

ExternalProject_Get_Property(json11_lib source_dir)
ExternalProject_Get_Property(json11_lib binary_dir)

link_directories(${binary_dir})
include_directories(${source_dir})

add_library(crestapi SHARED centreon-api.cc)
add_executable(ctest main.cc)
add_dependencies(ctest restclient-cpp json11_lib crestapi)
link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(ctest crestapi -lrestclient-cpp -ljson11)

add_executable(cgraph graph.cc)
target_link_libraries(cgraph crestapi -lrestclient-cpp -ljson11)
