## lib
SET (SRC_FETZER_LIB
    fm/literals.cpp
    fm/werckmeister.cpp
    fm/midi.cpp
    fm/common.cpp
    fm/exception.cpp
    fm/config/configServer.cpp
    fm/lua/ALuaObject.cpp
    fm/lua/ALuaScript.cpp
    compiler/compiler.cpp
    parser/chordDefParser.cpp
    parser/sheetParser.cpp
    parser/documentParser.cpp
    parser/pitchmapParser.cpp
    compiler/context/MidiContext.cpp
    compiler/context/AContext.cpp
    compiler/preprocessor.cpp
    compiler/sheetEventRenderer.cpp
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
    compiler/sheetTemplateRenderer.cpp
    compiler/error.cpp
    compiler/warning.cpp
    compiler/argumentNames.cpp
    compiler/lua/luaTimeInfo.cpp
    sheet/Event.cpp
    sheet/Document.cpp
    sheet/ChordDef.cpp
    sheet/tools.cpp
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
    fmapp/rtmidiBackend.cpp
    fmapp/midiplayer.cpp
    fmapp/midiProvider.cpp
    fmapp/udpSender.cpp
    fmapp/json.cpp
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
    sheetplayer_main.cpp 
    sheet.cpp
)

SET (SHEET_LIBS ${SHEET_LIBS} sheet fmapp lua ${Boost_LIBRARIES})

if(WIN32)
    SET (SHEET_LIBS ${SHEET_LIBS} Bcrypt.lib)
endif(WIN32)

add_library(fmapp ${SRC_SHEET_FMAPP})

add_executable(rehearse ${SRC_FETZER_TESTS})
target_link_libraries (rehearse ${SHEET_LIBS})

add_executable(sheetc sheetcompiler_main.cpp fmapp/json.cpp sheet.cpp)
target_link_libraries (sheetc ${SHEET_LIBS})

if (WIN32)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
endif(WIN32)
if (APPLE)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
elseif(UNIX)
    if ($ENV{WM_LINUX_LINK_STATIC})
        SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
    else($ENV{WM_LINUX_LINK_STATIC})
        SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi pthread)
    endif($ENV{WM_LINUX_LINK_STATIC})
endif(APPLE)

add_executable(sheetp ${SRC_SHEET_PLAYER})
target_link_libraries (sheetp ${SHEET_LIBS} ${LIBS_PLAYER})

SET (WM_TARGETS sheetp sheetc)

# Win32 Kill Process Workaround
if (WIN32) 
    add_executable(win32-kill-sheetp-process win32_kill_process_main.cpp)
    target_link_libraries (win32-kill-sheetp-process ${Boost_LIBRARIES})
    SET (WM_TARGETS ${WM_TARGETS} win32-kill-sheetp-process)
endif(WIN32)


install(TARGETS ${WM_TARGETS} DESTINATION bin)