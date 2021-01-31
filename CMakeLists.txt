# WM_DOWNLOAD_EXTERNALS=1 WM_LINUX_LINK_STATIC=1 cmake ..
cmake_minimum_required(VERSION 3.11)

execute_process(COMMAND git --work-tree=. log --pretty=format:%h -n 1
              OUTPUT_VARIABLE GIT_VERSION)
# Project name.
project(werckmeister)

SET(WERCKMEISTER_VERSION_MAJOR 0)
SET(WERCKMEISTER_VERSION_MINOR 1)
SET(WERCKMEISTER_VERSION_PATCH 56)

# Add this project's cmake/ directory to the module path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


# PROJECT CONFIG
set(FM_CHARSET "ISO-8859-1")
set(FM_PPQ 5000)
set(BOOST_VERSION "1.72.0")
set(FLUIDSYNTH_VERSION "2.1.0")
#END PROJECT CONFIG

set (INSTALL_AUX 1)

if ("${CMAKE_SYSTEM}" MATCHES "Emscripten*")
    MESSAGE("using emscripten")
    SET(USE_EMSCRIPTEN 1)
    SET (INSTALL_AUX 0)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/werckmeisterjs)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIB_FLUIDSYNTH_SEARCH_PATHS "")


IF(UNIX)
  SET (LIB_FLUIDSYNTH_FILENAME "libfluidsynth.so")
  SET (LIB_FLUIDSYNTH_SEARCH_PATHS "{\"/usr/lib\", \"/usr/local/lib\"}")
  SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -Wall -Wno-unused-function")
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
  include(DownloadLibFluidSynthWin)
  DownloadLibFluidSynthWin("${FLUIDSYNTH_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/libfluidsynth")
  SET (LIB_FLUIDSYNTH_FILENAME "libfluidsynth-2.dll")
  SET (LIB_FLUIDSYNTH_SEARCH_PATHS "{\"C:\\\\Windows\\\\System32\"}")
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
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250")
ENDIF(WIN32)


IF(APPLE)
  SET (LIB_FLUIDSYNTH_FILENAME "libfluidsynth.dylib")
  SET (LIB_FLUIDSYNTH_SEARCH_PATHS "{\"/usr/lib\", \"/usr/local/lib\"}")
  SET(Boost_USE_STATIC_LIBS ON)
ENDIF(APPLE)

SET(BOOST_PACKAGES unit_test_framework filesystem program_options locale system)

if ("$ENV{WM_DOWNLOAD_EXTERNALS}")
  #Download Boost
  include(DownloadBoost)
  DownloadBoost("${BOOST_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/boost" 0)
endif ()

if(${USE_EMSCRIPTEN})
  include(DownloadBoost)
  DownloadBoost("${BOOST_VERSION}" "${CMAKE_SOURCE_DIR}/third_party/boost" 1)
  add_compile_options(-s DISABLE_EXCEPTION_CATCHING=0 -Oz)
  # due a bug: https://github.com/emscripten-core/emscripten/issues/10078
  # we cant rely on find boost in combination with emscripten
  SET (Boost_INCLUDE_DIRS ${BOOST_ROOT})
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  # compile the needed files manually
  # (I was not able to get linking against the boost library to work properly, even with --toolset=emscripten)
  add_subdirectory(third_party/emscripten_boost)
else()
  find_package(Boost 1.66.0 COMPONENTS ${BOOST_PACKAGES} REQUIRED)
endif()

IF(APPLE)
  SET(Boost_LIBRARIES ${Boost_LIBRARIES}
      /usr/local/opt/icu4c/lib/libicudata.a 
      /usr/local/opt/icu4c/lib/libicui18n.a 
      /usr/local/opt/icu4c/lib/libicutu.a 
      /usr/local/opt/icu4c/lib/libicuuc.a 
      /usr/local/opt/icu4c/lib/libicuio.a 
      iconv)
ELSEIF(UNIX)
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
INCLUDE_DIRECTORIES(src/ext/fluidsynth)
INCLUDE_DIRECTORIES(submodules/lua/src)
INCLUDE_DIRECTORIES(submodules/rapidjson/include)
INCLUDE_DIRECTORIES(submodules/di/include)
INCLUDE_DIRECTORIES(submodules/di/extension/include)

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

if (${USE_EMSCRIPTEN})
  configure_file("package.in.json" "${CMAKE_BINARY_DIR}/package.json")
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_VENDOR "Samba Godschynski")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${WERCKMEISTER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WERCKMEISTER_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WERCKMEISTER_VERSION_PATCH}")
if(WIN32)
  set(CPACK_GENERATOR "WIX")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/installer_assets/wix_env.wxs")
  set(CPACK_WIX_UI_DIALOG "${PROJECT_SOURCE_DIR}/installer_assets/Logo_UI.bmp")
  set(CPACK_WIX_UPGRADE_GUID  "C2C29CB5-2FB4-4367-83C5-A3F41D3502BC")
endif(WIN32)

include(CPack)

set (EXAMPLES_FOLDER "${PROJECT_SOURCE_DIR}/examples")

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

if(WIN32)
  file(GLOB LIB_FLUIDSYNTH_FILES
      "${FLUIDSYNTH_ROOT}/bin/*.dll"
  )
  install(FILES ${LIB_FLUIDSYNTH_FILES}    DESTINATION bin)
endif()

if(${INSTALL_AUX})
  install(FILES ${WM_CHORDS}                 DESTINATION share/werckmeister/chords)
  install(FILES ${WM_LUA}                    DESTINATION share/werckmeister/lua)
  install(FILES ${WM_LUA_COM}                DESTINATION share/werckmeister/lua/com)
  install(FILES ${WM_LUA_MODS}               DESTINATION share/werckmeister/lua/mods)
  install(FILES ${WM_LUA_VOICINGS}           DESTINATION share/werckmeister/lua/voicings)
  install(FILES ${WM_LUA_VOICINGS_SOVLERS}   DESTINATION share/werckmeister/lua/voicings/solvers)
  install(FILES ${WM_PITCHMAPS}              DESTINATION share/werckmeister/pitchmaps)
  install(FILES ${WM_TEMPLATES}              DESTINATION share/werckmeister/templates)
endif()

if (${USE_EMSCRIPTEN})
  execute_process(WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND python3 ${CMAKE_SOURCE_DIR}/packauxiliaries.py
    ${EXAMPLES_FOLDER}
    ${WM_CHORDS} 
    ${WM_LUA} 
    ${WM_LUA_COM}
    ${WM_LUA_MODS}
    ${WM_LUA_VOICINGS}
    ${WM_LUA_VOICINGS_SOVLERS}
    ${WM_PITCHMAPS}
    ${WM_TEMPLATES})

  install(FILES ${CMAKE_BINARY_DIR}/werckmeister-auxiliaries.json DESTINATION .)
endif()
