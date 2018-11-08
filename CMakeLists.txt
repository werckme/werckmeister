cmake_minimum_required(VERSION 2.8.12.1)

execute_process(COMMAND git --work-tree=. log --pretty=format:%h -n 1
              OUTPUT_VARIABLE GIT_VERSION)
# Project name.
project(sheet)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

IF(UNIX)
  SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -Wall")
ENDIF(UNIX)

IF(WIN32)
  add_definitions( -DBOOST_ALL_NO_LIB )
  SET (BOOST_ROOT "C:\\Users\\samba\\libs\\boost_1_66_0")
  SET (BOOST_LIBRARYDIR "C:\\Users\\samba\\libs\\boost_1_66_0\\stage\\lib")
  SET(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)
ENDIF(WIN32)

find_package(Boost COMPONENTS unit_test_framework filesystem program_options REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src/)
INCLUDE_DIRECTORIES(src/ext)
INCLUDE_DIRECTORIES(submodules/rtmidi)

#add_subdirectory(submodules/rtmidi) #rtmidi needs /MT or /MTd for windows
add_subdirectory(src)

