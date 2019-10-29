cmake_minimum_required(VERSION 3.13)
add_subdirectory(cpr)

project(mcrd)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 14)
find_package(Boost COMPONENTS program_options log REQUIRED)
find_package(Threads REQUIRED)

FILE(GLOB SRCFILES *.cpp)
ADD_EXECUTABLE(mcrd ${SRCFILES})

target_link_libraries(mcrd Threads::Threads)
target_link_libraries(mcrd Boost::program_options)
target_link_libraries(mcrd Boost::log)
target_link_libraries(mcrd ${Boost_LIBRARIES})
