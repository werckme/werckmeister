set(CMAKE_BUILD_TYPE Debug)

## lib
SET (SRC_FETZER_LIB
    fm/literals.cpp
    fm/werckmeister.cpp
    fm/midi.cpp
    fm/common.cpp
    fm/config/configServer.cpp
    compiler/compiler.cpp
    compiler/chordDefParser.cpp
    compiler/sheetParser.cpp
    compiler/documentConfigParser.cpp
    compiler/documentParser.cpp
    compiler/pitchmapParser.cpp
    compiler/MidiContext.cpp
    compiler/AContext.cpp
    compiler/spielanweisung/Normal.cpp
    compiler/spielanweisung/ASpielanweisung.cpp
    compiler/spielanweisung/Arpeggio.cpp
    compiler/spielanweisung/Vorschlag.cpp
    compiler/modification/AModification.cpp
    compiler/modification/Bend.cpp
    compiler/voicings/DirectVoicingStrategy.cpp
    compiler/voicings/SimpleGuitar.cpp
    sheet/Event.cpp
    sheet/Document.cpp
    sheet/ChordDef.cpp
)


if (WIN32)
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(WIN32)
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
)

## PLAYER
SET (SRC_SHEET_PLAYER
    fmapp/boostTimer.cpp
    fmapp/rtmidiBackend.cpp
    fmapp/midiplayer.cpp
    fmapp/midiProvider.cpp
    sheetplayer_main.cpp 
    sheet.cpp
)

## PLAYER
SET (SRC_SHEET_TESTER
    fmapp/boostTimer.cpp
    fmapp/rtmidiBackend.cpp
    fmapp/midiplayer.cpp
    fmapp/midiProvider.cpp
    sheettest_main.cpp
    sheet.cpp
)

if (WIN32)
  SET(SRC_SHEET_PLAYER ${SRC_SHEET_PLAYER}  fmapp/os_win.cpp)
  SET(SRC_SHEET_TESTER ${SRC_SHEET_TESTER}  fmapp/os_win.cpp)
endif(WIN32)

if (UNIX)
  SET(SRC_SHEET_PLAYER ${SRC_SHEET_PLAYER}  fmapp/os_linux.cpp)
  SET(SRC_SHEET_TESTER ${SRC_SHEET_TESTER}  fmapp/os_linux.cpp)
endif(UNIX)

SET (SHEET_LIBS ${SHEET_LIBS} sheet ${Boost_LIBRARIES} lua)

add_executable(rehearse ${SRC_FETZER_TESTS})
target_link_libraries (rehearse ${SHEET_LIBS})

add_executable(sheetc sheetcompiler_main.cpp sheet.cpp)
target_link_libraries (sheetc ${SHEET_LIBS})

if (WIN32)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi_static)
endif(WIN32)
if (UNIX)
    SET(LIBS_PLAYER ${LIBS_PLAYER} rtmidi pthread)
endif(UNIX)

add_executable(sheetp ${SRC_SHEET_PLAYER})
target_link_libraries (sheetp ${SHEET_LIBS} ${LIBS_PLAYER})
