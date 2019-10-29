cmake_minimum_required(VERSION 3.10)

project(mcrd)
if(FOO_USE_EXTERNAL_JSON)
  find_package(nlohmann_json 3.2.0 REQUIRED)
else()
  set(JSON_BuildTests OFF CACHE INTERNAL "")
  set(JSON_Install OFF CACHE INTERNAL "")
  add_subdirectory(nlohmann)
endif()

# Boost

if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
  CMAKE_POLICY(SET CMP0074 NEW)
  find_package(Boost 1.71.0 COMPONENTS program_options log REQUIRED)
else()
  find_package(Boost COMPONENTS program_options log REQUIRED)
  add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

 set(PROGNAME mcrd)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

FILE(GLOB SRCFILES *.cpp)
ADD_EXECUTABLE(${PROGNAME} ${SRCFILES})

target_link_libraries(${PROGNAME} Threads::Threads)
target_link_libraries(${PROGNAME} Boost::program_options)
target_link_libraries(${PROGNAME} Boost::log)
target_link_libraries(${PROGNAME} ${Boost_LIBRARIES})
target_link_libraries(${PROGNAME} nlohmann_json::nlohmann_json)