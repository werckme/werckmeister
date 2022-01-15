# Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

# Download the Boost distribution for |version| to
# |download_dir|. The |BOOST_ROOT| variable will be set in global scope pointing
# to the extracted location.

function(DownloadBOOST version download_dir skip_build)
  # Specify the binary distribution type and download directory.
  string(REPLACE "." "_" version_lodash ${version})
  set(BOOST_DISTRIBUTION "boost_${version_lodash}")
  set(BOOST_DOWNLOAD_DIR "${download_dir}")

  # The location where we expect the extracted binary distribution.
  set(BOOST_ROOT "${BOOST_DOWNLOAD_DIR}/${BOOST_DISTRIBUTION}" CACHE INTERNAL "BOOST_ROOT")

  set(BOOST_PREFIX "${BOOST_ROOT}")
  set(TOOLSET "")
  if(${USE_EMSCRIPTEN})
    message(STATUS "Using Toolset Emscripten")
    SET(TOOLSET "--toolset=emscripten")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Using Toolset Clang")
    SET(TOOLSET "--toolset=clang")
  endif()
  
  if(WIN32)
    SET(FILE_EXT ".zip")
    SET(BOOST_BOOTSTRAP "bootstrap.bat")
    SET(BOOST_B2 "b2")
  else(WIN32)
    SET(FILE_EXT ".tar.gz")
    SET(BOOST_BOOTSTRAP "./bootstrap.sh")
    SET(BOOST_B2 "./b2")
  endif(WIN32)

  # Download and/or extract the binary distribution if necessary.
  if(NOT IS_DIRECTORY "${BOOST_ROOT}")
    set(BOOST_DOWNLOAD_FILENAME "${BOOST_DISTRIBUTION}${FILE_EXT}")
    set(BOOST_DOWNLOAD_PATH "${BOOST_DOWNLOAD_DIR}/${BOOST_DOWNLOAD_FILENAME}")
    if(NOT EXISTS "${BOOST_DOWNLOAD_PATH}")
      set(BOOST_DOWNLOAD_URL "https://boostorg.jfrog.io/artifactory/main/release/${version}/source/${BOOST_DOWNLOAD_FILENAME}")
      # Download the binary distribution and verify the hash.
      message(STATUS "Downloading ${BOOST_DOWNLOAD_URL}...")
      file(
        DOWNLOAD "${BOOST_DOWNLOAD_URL}" "${BOOST_DOWNLOAD_PATH}"
        SHOW_PROGRESS
      )
    endif()

    # Extract the binary distribution.
    message(STATUS "Extracting ${BOOST_DOWNLOAD_PATH}...")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar xzf "${BOOST_DOWNLOAD_DIR}/${BOOST_DOWNLOAD_FILENAME}"
      WORKING_DIRECTORY ${BOOST_DOWNLOAD_DIR}
    )
    if(skip_build)
      return()
    endif()
    message(STATUS "Executing ${BOOST_BOOTSTRAP}...")
    execute_process(
      COMMAND ${BOOST_BOOTSTRAP}
      WORKING_DIRECTORY ${BOOST_ROOT}
    )
    message(STATUS "building boost...")
    execute_process(
      COMMAND ${BOOST_B2}
      ${TOOLSET}
      "link=static" 
      "runtime-link=static" 
      "--with-filesystem" 
      "--with-program_options" 
      "--with-locale" 
      "--with-system"
      "--with-test"
      "--with-date_time"
      WORKING_DIRECTORY ${BOOST_ROOT}
    )
  
  endif()

endfunction()
