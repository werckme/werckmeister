## lib
SET (SRC_FETZER_LIB
    fm/literals.cpp
    fm/werckmeister.cpp
    fm/midi.cpp
    fm/common.cpp
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
)

add_executable(rehearse ${SRC_FETZER_TESTS})
target_link_libraries (rehearse sheet ${Boost_LIBRARIES})
