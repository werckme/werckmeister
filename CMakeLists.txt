cmake_minimum_required(VERSION 2.8.12.1)

execute_process(COMMAND git --work-tree=. log --pretty=format:%h -n 1
              OUTPUT_VARIABLE GIT_VERSION)
# Project name.
project(sheet)

# PROJECT CONFIG
set(FM_CHARSET "ISO-8859-1")
set(FM_PPQ 500)
#END PROJECT CONFIG

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

IF(UNIX)
  SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -Wall")
  add_definitions( -DSHEET_USE_BOOST_TIMER )
  add_definitions( -DSHEET_OS_UNIX )
#  SET(Boost_USE_STATIC_LIBS ON)
#  set(Boost_USE_MULTITHREADED ON)
#  set(Boost_USE_STATIC_RUNTIME ON)


ENDIF(UNIX)

IF(WIN32)
  add_definitions( -DSHEET_USE_BOOST_TIMER )
  add_definitions( -DBOOST_ALL_NO_LIB )
  add_definitions( -DSHEET_OS_WINDOWS )
  SET (BOOST_ROOT "C:\\Users\\samba\\libs\\boost_1_66_0")
  SET (BOOST_LIBRARYDIR "C:\\Users\\samba\\libs\\boost_1_66_0\\stage\\lib")
  SET(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)
ENDIF(WIN32)

find_package(Boost COMPONENTS unit_test_framework filesystem program_options locale REQUIRED)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src/)
INCLUDE_DIRECTORIES(src/ext)
INCLUDE_DIRECTORIES(submodules/rtmidi)

add_subdirectory(submodules/rtmidi) #rtmidi needs /MT or /MTd for windows
add_subdirectory(src)

configure_file("${PROJECT_SOURCE_DIR}/src/fm/config.hpp.in" "${PROJECT_SOURCE_DIR}/src/fm/config.hpp")

