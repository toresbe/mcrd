cmake_minimum_required(VERSION 3.10)

project(mcrd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(thirdparty/json)

# Date API
add_subdirectory(thirdparty/date)
include_directories("thirdparty/date/include")

# Boost & Curl

if(WIN32)
  find_package(CURL CONFIG REQUIRED)
  set(Boost_USE_STATIC_LIBS ON)
  CMAKE_POLICY(SET CMP0074 NEW)
  find_package(Boost 1.71.0 COMPONENTS program_options log REQUIRED)
else()
  find_package(CURL REQUIRED)
  find_package(Boost COMPONENTS program_options log REQUIRED)
  add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

set(PROGNAME mcrd)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Threads REQUIRED)

FILE(GLOB SRCFILES *.cpp
    device/*.cpp
    misc/*.cpp
    schedule/*.cpp
    workers/*.cpp)

ADD_EXECUTABLE(${PROGNAME} ${SRCFILES})

set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++17 standard compliance
    CXX_STANDARD 17
)

target_link_libraries(${PROGNAME} Threads::Threads)
target_link_libraries(${PROGNAME} Boost::program_options)
target_link_libraries(${PROGNAME} Boost::log)
target_link_libraries(${PROGNAME} ${Boost_LIBRARIES})
target_link_libraries(${PROGNAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROGNAME} CURL::libcurl)
