#find boost/ -regex ".*filesystem.*cpp"
SET(BOOST_SOURCES
    ${BOOST_ROOT}/libs/filesystem/src/windows_file_codecvt.cpp
    ${BOOST_ROOT}/libs/filesystem/src/path.cpp
    ${BOOST_ROOT}/libs/filesystem/src/exception.cpp
    ${BOOST_ROOT}/libs/filesystem/src/directory.cpp
    ${BOOST_ROOT}/libs/filesystem/src/portability.cpp
    ${BOOST_ROOT}/libs/filesystem/src/unique_path.cpp
    ${BOOST_ROOT}/libs/filesystem/src/codecvt_error_category.cpp
    ${BOOST_ROOT}/libs/filesystem/src/utf8_codecvt_facet.cpp
    ${BOOST_ROOT}/libs/filesystem/src/path_traits.cpp
    ${BOOST_ROOT}/libs/filesystem/src/operations.cpp

    ${BOOST_ROOT}/libs/program_options/src/winmain.cpp
    ${BOOST_ROOT}/libs/program_options/src/split.cpp
    ${BOOST_ROOT}/libs/program_options/src/value_semantic.cpp
    ${BOOST_ROOT}/libs/program_options/src/options_description.cpp
    ${BOOST_ROOT}/libs/program_options/src/config_file.cpp
    ${BOOST_ROOT}/libs/program_options/src/parsers.cpp
    ${BOOST_ROOT}/libs/program_options/src/variables_map.cpp
    ${BOOST_ROOT}/libs/program_options/src/cmdline.cpp
    ${BOOST_ROOT}/libs/program_options/src/positional_options.cpp
    ${BOOST_ROOT}/libs/program_options/src/utf8_codecvt_facet.cpp
    ${BOOST_ROOT}/libs/program_options/src/convert.cpp
)

add_library(emscripten_boost_libs ${BOOST_SOURCES})