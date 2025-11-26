function(DownloadLibFluidSynthWin version download_dir)
  # Specify the binary distribution type and download directory.
  set(FLUIDSYNTH_DOWNLOAD_DIR "${download_dir}")

  set(FLUIDSYNTH_ROOT "${FLUIDSYNTH_DOWNLOAD_DIR}" CACHE INTERNAL "FLUIDSYNTH_ROOT")
 
  # Download and/or extract the binary distribution if necessary.
  if(NOT IS_DIRECTORY "${FLUIDSYNTH_DOWNLOAD_DIR}")
    ##https://github.com/FluidSynth/fluidsynth/releases/download/v2.1.0/fluidsynth-2.1.0-win64.zip
    set(FLUIDSYNTH_DOWNLOAD_FILENAME "fluidsynth-${version}-win10-x64.zip")
    set(FLUIDSYNTH_DOWNLOAD_PATH "${FLUIDSYNTH_DOWNLOAD_DIR}/${FLUIDSYNTH_DOWNLOAD_FILENAME}")
    if(NOT EXISTS "${FLUIDSYNTH_DOWNLOAD_PATH}")
      set(FLUIDSYNTH_DOWNLOAD_URL "https://github.com/FluidSynth/fluidsynth/releases/download/v${version}/${FLUIDSYNTH_DOWNLOAD_FILENAME}")
      message(STATUS "Downloading ${FLUIDSYNTH_DOWNLOAD_URL}...")
      file(
        DOWNLOAD "${FLUIDSYNTH_DOWNLOAD_URL}" "${FLUIDSYNTH_DOWNLOAD_PATH}"
        SHOW_PROGRESS
      )
    endif()

    # Extract the binary distribution.
    message(STATUS "Extracting ${FLUIDSYNTH_DOWNLOAD_PATH}...")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E tar xzf "${FLUIDSYNTH_DOWNLOAD_DIR}/${FLUIDSYNTH_DOWNLOAD_FILENAME}"
      WORKING_DIRECTORY ${FLUIDSYNTH_DOWNLOAD_DIR}
    )
  endif()

endfunction()
