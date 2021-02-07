if(${USE_EMSCRIPTEN})
    SET(BUILD_SHEETJS 1)
else()
    SET(BUILD_SHEETC   1)
    SET(BUILD_SHEETJSD 1)
    SET(BUILD_SHEETP   1)
    SET(BUILD_REHEARSE 1)
endif()

## lib
SET (SRC_FETZER_LIB
    fm/literals.cpp
    fm/werckmeister.cpp
    fm/midi.cpp
    fm/common.cpp
    fm/exception.cpp
    fm/AConvertsArgumentsToParameter.cpp
    fm/config/configServer.cpp
    fm/lua/ALuaObject.cpp
    fm/lua/ALuaScript.cpp
    fm/lua/ALuaWithParameter.cpp
    fm/DefinitionsServer.cpp
    fm/tools.cpp
    fm/ConsoleLogger.cpp
    compiler/Compiler.cpp
    parser/chordDefParser.cpp
    parser/sheetParser.cpp
    parser/documentParser.cpp
    parser/pitchmapParser.cpp
    compiler/context/MidiContext.cpp
    compiler/context/AContext.cpp
    compiler/Preprocessor.cpp
    compiler/SheetEventRenderer.cpp
    compiler/spielanweisung/ASpielanweisung.cpp
    compiler/spielanweisung/Arpeggio.cpp
    compiler/spielanweisung/Vorschlag.cpp
    compiler/modification/AModification.cpp
    compiler/modification/Bend.cpp
    compiler/modification/LuaMod.cpp
    compiler/voicings/DirectVoicingStrategy.cpp
    compiler/voicings/SimpleGuitar.cpp
    compiler/voicings/Lua.cpp
    compiler/metaData.cpp
    compiler/SheetTemplateRenderer.cpp
    compiler/error.cpp
    compiler/Warning.cpp
    compiler/argumentNames.cpp
    compiler/SheetNavigator.cpp
    compiler/lua/luaTimeInfo.cpp
    sheet/objects/Event.cpp
    sheet/objects/ChordDef.cpp
    sheet/Document.cpp
    compiler/commands/ACommand.cpp
    compiler/commands/ACanSpecifyInstrument.cpp
    compiler/commands/DefineMidiInstrument.cpp
    compiler/commands/SetVolume.cpp
    compiler/commands/SetPan.cpp
    compiler/commands/SetTrackType.cpp
    compiler/commands/SetTrackName.cpp
    compiler/commands/AddVorschlag.cpp
    compiler/commands/SetTemplatePosition.cpp
    compiler/commands/SetTemplate.cpp
    compiler/commands/SetExpression.cpp
    compiler/commands/SetExpressionPlayedOnce.cpp
    compiler/commands/SetSignature.cpp
    compiler/commands/SetTempo.cpp
    compiler/commands/SetVoicingStrategy.cpp
    compiler/commands/SetSpielanweisung.cpp
    compiler/commands/SetSpielanweisung.cpp
    compiler/commands/SetSpielanweisungPlayedOnce.cpp
    compiler/commands/AddMod.cpp
    compiler/commands/AddModPlayedOnce.cpp
    compiler/commands/SetInstrument.cpp
    compiler/commands/SelectMidiSound.cpp
    compiler/commands/AddInstrumentConfig.cpp
    compiler/commands/InstrumentConfigSetPan.cpp
    compiler/commands/InstrumentConfigAddMod.cpp
    compiler/commands/InstrumentConfigSetVoicingStrategy.cpp
    compiler/commands/InstrumentConfigSetVolume.cpp    
    compiler/commands/InstrumentConfigVelocityRemap.cpp    
    compiler/commands/AddDevice.cpp
    compiler/commands/Mark.cpp    
    compiler/commands/Jump.cpp 
)

add_definitions( -DGIT_VERSION="${GIT_VERSION}" )
add_library(sheet ${SRC_FETZER_LIB})

## TESTS
SET (SRC_FETZER_TESTS
    tests/unit_test.cpp
    tests/midi_tests.cpp
    tests/test_common.cpp
    tests/test_lexer.cpp
    tests/test_parser.cpp
    tests/longString1.cpp
    tests/test_bugfixes.cpp
    tests/testhelper.cpp
    tests/test_document.cpp
)

## FMAPP
SET (SRC_SHEET_FMAPP
    fmapp/boostTimer.cpp
    fmapp/ADocumentWriter.cpp
    fmapp/MidiFileWriter.cpp
    fmapp/midiProvider.cpp
    fmapp/UdpSender.cpp
    fmapp/JsonWriter.cpp
    fmapp/JsonStringInputReader.cpp
    fmapp/JsonIOBase.cpp
    fmapp/SheetWatcher.cpp
    fmapp/PlayerTimePrinter.cpp
    fmapp/Funkfeuer.cpp
)

if (WIN32)
  SET(SRC_SHEET_FMAPP ${SRC_SHEET_FMAPP}  fmapp/os_win.cpp)
endif(WIN32)

if (APPLE)
  SET(SRC_SHEET_FMAPP ${SRC_SHEET_FMAPP}  fmapp/os_mac.cpp)
elseif(UNIX)
  SET(SRC_SHEET_FMAPP ${SRC_SHEET_FMAPP}  fmapp/os_linux.cpp)
endif(APPLE)


## PLAYER
SET (SRC_SHEET_PLAYER
    fmapp/MidiPlayer.cpp
    fmapp/RTMidiBackend.cpp
    fmapp/FluidSynthBackend.cpp
    fmapp/FluidSynthWrapper.cpp
    fmapp/MidiBackendContainer.cpp
    sheetplayer_main.cpp 
    SheetCompilerProgram.cpp 
    SheetPlayerProgram.cpp 
    CompilerProgramOptions.cpp
    PlayerProgramOptions.cpp
)

SET (SHEET_LIBS ${SHEET_LIBS} fmapp sheet lua ${Boost_LIBRARIES})

if(WIN32)
    SET (SHEET_LIBS ${SHEET_LIBS} Bcrypt.lib)
endif(WIN32)

add_library(fmapp ${SRC_SHEET_FMAPP})

if(${BUILD_REHEARSE})
    add_executable(rehearse ${SRC_FETZER_TESTS})
    target_link_libraries (rehearse ${SHEET_LIBS})
endif()

if(${BUILD_SHEETC})
    add_executable(sheetc sheetcompiler_main.cpp SheetCompilerProgram.cpp CompilerProgramOptions.cpp)
    target_link_libraries (sheetc ${SHEET_LIBS})
    SET (WM_TARGETS ${WM_TARGETS} sheetc)
endif()

if(${BUILD_SHEETJSD})
    add_executable(sheetjsc sheetjs_main.cpp SheetCompilerProgramJs.cpp SheetCompilerProgram.cpp)
    target_link_libraries (sheetjsc ${SHEET_LIBS})
    SET (WM_TARGETS ${WM_TARGETS} sheetc)
endif()

if(${BUILD_SHEETJS})
    add_executable(werckmeister sheetjs_main.cpp SheetCompilerProgramJs.cpp SheetCompilerProgram.cpp)
    target_link_libraries (werckmeister ${SHEET_LIBS} emscripten_boost_libs)
    set_target_properties(werckmeister PROPERTIES LINK_FLAGS "-Oz -s MODULARIZE=1 -s EXPORT_NAME=\"startWerckmeisterJs\" -s DISABLE_EXCEPTION_CATCHING=0 -s EXTRA_EXPORTED_RUNTIME_METHODS=\"['cwrap', 'UTF8ToString', 'FS']\" -s EXPORTED_FUNCTIONS=\"['_main', '_create_compile_result']\"")
    install(FILES ${CMAKE_BINARY_DIR}/werckmeister.wasm DESTINATION .)
    install(FILES ${CMAKE_BINARY_DIR}/package.json DESTINATION .)
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION .)
    install(TARGETS werckmeister DESTINATION .)
endif()


if (WIN32)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
endif(WIN32)
if (APPLE)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
elseif(UNIX)
    if ($ENV{WM_LINUX_LINK_STATIC})
        SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static ${CMAKE_DL_LIBS})
    else($ENV{WM_LINUX_LINK_STATIC})
        SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi pthread ${CMAKE_DL_LIBS})
    endif($ENV{WM_LINUX_LINK_STATIC})
endif(APPLE)

if(${BUILD_SHEETP})
    add_executable(sheetp ${SRC_SHEET_PLAYER})
    target_link_libraries (sheetp ${SHEET_LIBS} ${LIBS_PLAYER})
    SET (WM_TARGETS ${WM_TARGETS} sheetp)
endif()



# Win32 Kill Process Workaround
if (WIN32) 
    add_executable(win32-kill-sheetp-process win32_kill_process_main.cpp)
    target_link_libraries (win32-kill-sheetp-process ${Boost_LIBRARIES})
    SET (WM_TARGETS ${WM_TARGETS} win32-kill-sheetp-process)
endif(WIN32)

# Win32 LibFluidSynth copy
if(WIN32)
  add_custom_command(TARGET sheetp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${FLUIDSYNTH_ROOT}/bin "$<TARGET_FILE_DIR:sheetp>"
  )  
endif()


install(TARGETS ${WM_TARGETS} DESTINATION bin)
