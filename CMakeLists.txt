cmake_minimum_required( VERSION 3.22.0 )

project(    vpr-net
            VERSION         1.0.1
            LANGUAGES       CXX )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory( tests/mock )

enable_testing()
add_test(NAME Test COMMAND tests/test.py)