cmake_minimum_required(VERSION 2.8.12.1)

execute_process(COMMAND git --work-tree=. log --pretty=format:%h -n 1
              OUTPUT_VARIABLE GIT_VERSION)
# Project name.
project(werckmeister)

SET(WERCKMEISTER_VERSION_MAJOR 0)
SET(WERCKMEISTER_VERSION_MINOR 1)
SET(WERCKMEISTER_VERSION_PATCH 5)

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
  if ($ENV{WM_LINUX_LINK_STATIC})
    message(STATUS "USE BOOST STATIC LIBS")
    SET(Boost_USE_STATIC_LIBS ON)
  endif($ENV{WM_LINUX_LINK_STATIC})
#  set(Boost_USE_MULTITHREADED ON)
#  set(Boost_USE_STATIC_RUNTIME ON)
ENDIF(UNIX)

IF(WIN32)
  add_definitions( -DSHEET_USE_BOOST_TIMER )
  add_definitions( -DBOOST_ALL_NO_LIB )
  add_definitions( -DSHEET_OS_WINDOWS )
  SET(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
ENDIF(WIN32)


IF(APPLE)
  SET(Boost_USE_STATIC_LIBS ON)
ENDIF(APPLE)

find_package(Boost 1.66.0 COMPONENTS unit_test_framework filesystem program_options locale system REQUIRED)

IF(APPLE)
  SET(Boost_LIBRARIES ${Boost_LIBRARIES}
      /usr/local/opt/icu4c/lib/libicudata.a 
      /usr/local/opt/icu4c/lib/libicui18n.a 
      /usr/local/opt/icu4c/lib/libicutu.a 
      /usr/local/opt/icu4c/lib/libicuuc.a 
      /usr/local/opt/icu4c/lib/libicuio.a 
      iconv)
ENDIF(APPLE)

IF(UNIX)
  if ($ENV{WM_LINUX_LINK_STATIC})
    SET(
      Boost_LIBRARIES ${Boost_LIBRARIES}
      /usr/lib/x86_64-linux-gnu/libicui18n.a 
      /usr/lib/x86_64-linux-gnu/libicutu.a 
      /usr/lib/x86_64-linux-gnu/libicuuc.a 
      /usr/lib/x86_64-linux-gnu/libicuio.a
      /usr/lib/x86_64-linux-gnu/libicudata.a  
      dl)
  endif ($ENV{WM_LINUX_LINK_STATIC})
ENDIF(UNIX)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src/)
INCLUDE_DIRECTORIES(src/ext)
INCLUDE_DIRECTORIES(submodules/lua/src)
INCLUDE_DIRECTORIES(submodules/rapidjson/include/)

if(WIN32)
  INCLUDE_DIRECTORIES(submodules)
  add_subdirectory(submodules/rtmidi) #rtmidi needs /MT or /MTd for windows
endif(WIN32)

if(APPLE)
  INCLUDE_DIRECTORIES(submodules)
  add_subdirectory(submodules/rtmidi) #rtmidi needs /MT or /MTd for windows
elseif(UNIX)
  if($ENV{WM_LINUX_LINK_STATIC})
    INCLUDE_DIRECTORIES(submodules)
    add_subdirectory(submodules/rtmidi)
  endif($ENV{WM_LINUX_LINK_STATIC})
endif(APPLE)


add_subdirectory(src)
add_subdirectory(submodules/lua)

configure_file("${PROJECT_SOURCE_DIR}/src/fm/config.hpp.in" "${PROJECT_SOURCE_DIR}/src/fm/config.hpp")

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR "Samba Godschynski")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${WERCKMEISTER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WERCKMEISTER_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WERCKMEISTER_VERSION_PATCH}")
if(WIN32)
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/installer_assets/wix_env.wxs")
  set(CPACK_WIX_UI_DIALOG "${PROJECT_SOURCE_DIR}/installer_assets/Logo_UI.bmp")
  set(CPACK_WIX_UPGRADE_GUID  "C2C29CB5-2FB4-4367-83C5-A3F41D3502BC")
endif(WIN32)
include(CPack)


file(GLOB WM_CHORDS
    "${EXAMPLES_FOLDER}/chords/*.chords"
)
file(GLOB WM_LUA
    "${EXAMPLES_FOLDER}/lua/*.lua"
)
file(GLOB WM_LUA_COM
    "${EXAMPLES_FOLDER}/lua/com/*.lua"
)
file(GLOB WM_LUA_MODS
    "${EXAMPLES_FOLDER}/lua/mods/*.lua"
)
file(GLOB WM_LUA_VOICINGS
    "${EXAMPLES_FOLDER}/lua/voicings/*.lua"
)
file(GLOB WM_LUA_VOICINGS_SOVLERS
    "${EXAMPLES_FOLDER}/lua/voicings/solvers/*.lua"
)
file(GLOB WM_PITCHMAPS
    "${EXAMPLES_FOLDER}/pitchmaps/*.pitchmap"
)
file(GLOB WM_TEMPLATES
    "${EXAMPLES_FOLDER}/templates/*.template"
)

install(FILES ${WM_CHORDS}                 DESTINATION share/werckmeister/chords)
install(FILES ${WM_LUA}                    DESTINATION share/werckmeister/lua)
install(FILES ${WM_LUA_COM}                DESTINATION share/werckmeister/lua/com)
install(FILES ${WM_LUA_MODS}               DESTINATION share/werckmeister/lua/mods)
install(FILES ${WM_LUA_VOICINGS}           DESTINATION share/werckmeister/lua/voicings)
install(FILES ${WM_LUA_VOICINGS_SOVLERS}   DESTINATION share/werckmeister/lua/voicings/solvers)
install(FILES ${WM_PITCHMAPS}              DESTINATION share/werckmeister/pitchmaps)
install(FILES ${WM_TEMPLATES}              DESTINATION share/werckmeister/templates)
