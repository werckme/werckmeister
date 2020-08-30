#find boost/ -regex ".*filesystem.*cpp"
SET(BOOST_SOURCES
    boost/libs/filesystem/src/windows_file_codecvt.cpp
    boost/libs/filesystem/src/path.cpp
    boost/libs/filesystem/src/exception.cpp
    boost/libs/filesystem/src/directory.cpp
    boost/libs/filesystem/src/portability.cpp
    boost/libs/filesystem/src/unique_path.cpp
    boost/libs/filesystem/src/codecvt_error_category.cpp
    boost/libs/filesystem/src/utf8_codecvt_facet.cpp
    boost/libs/filesystem/src/path_traits.cpp
    boost/libs/filesystem/src/operations.cpp

    boost/libs/program_options/src/winmain.cpp
    boost/libs/program_options/src/split.cpp
    boost/libs/program_options/src/value_semantic.cpp
    boost/libs/program_options/src/options_description.cpp
    boost/libs/program_options/src/config_file.cpp
    boost/libs/program_options/src/parsers.cpp
    boost/libs/program_options/src/variables_map.cpp
    boost/libs/program_options/src/cmdline.cpp
    boost/libs/program_options/src/positional_options.cpp
    boost/libs/program_options/src/utf8_codecvt_facet.cpp
    boost/libs/program_options/src/convert.cpp
)

add_library(emscripten_boost_libs ${BOOST_SOURCES})