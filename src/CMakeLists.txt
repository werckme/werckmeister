## lib
SET (SRC_FETZER_LIB
    fm/literals.cpp
    fm/werckmeister.cpp
    fm/midi.cpp
    fm/common.cpp
    compiler/compiler.cpp
    compiler/chordDefParser.cpp
    compiler/sheetParser.cpp
    compiler/documentConfigParser.cpp
    compiler/documentParser.cpp
    compiler/MidiContext.cpp
    compiler/AContext.cpp
    sheet/Event.cpp
    sheet/Document.cpp
    sheet/DirectVoicingStrategy.cpp
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
)

## PLAYER
SET (SRC_SHEET_PLAYER
    fmapp/os_win.cpp
    fmapp/boostTimer.cpp
    fmapp/rtmidiBackend.cpp
    fmapp/midiplayer.cpp
    fmapp/midiProvider.cpp
    sheetplayer_main.cpp 
    sheet.cpp
)

add_executable(rehearse ${SRC_FETZER_TESTS})
target_link_libraries (rehearse sheet ${Boost_LIBRARIES})

add_executable(sheetc sheetcompiler_main.cpp sheet.cpp)
target_link_libraries (sheetc sheet ${Boost_LIBRARIES})

add_executable(sheetp ${SRC_SHEET_PLAYER})
target_link_libraries (sheetp sheet ${Boost_LIBRARIES} rtmidi_static)
