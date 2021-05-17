cmake_minimum_required(VERSION 3.0.0)
project(MathParser VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_compile_definitions(SFML_STATIC)
add_compile_options("-Wno-write-strings")
add_subdirectory(fmt)
# set(CMAKE_CXX_FLAGS "-O2")

set(SOURCES
    src/nix/tempfile.cpp
    src/nix/fork.cpp
    src/nix/pipe.cpp
    src/nix/dynamic_loader.cpp
    src/rng.cpp
    src/plot/Graph.cpp
    src/Functions.cpp
    src/aux.cpp
    src/Precedence.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/main.cpp
)

include(CTest)
enable_testing()

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(fmt)
add_executable(MathParser ${SOURCES})
target_include_directories(MathParser
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(MathParser
    fmt::fmt
    sfml-graphics
    sfml-window
    sfml-system
    pthread
    dl
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
