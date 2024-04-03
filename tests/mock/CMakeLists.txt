set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_CXX_STANDARD 20)

include_directories( ${CMAKE_SOURCE_DIR} )

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<CXX_COMPILER_ID:MSVC>:/WX>
            $<$<CXX_COMPILER_ID:MSVC>:/MT>

)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ws2_32 secur32
)