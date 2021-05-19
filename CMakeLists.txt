cmake_minimum_required(VERSION 3.0.0)
project(MathParser VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(SINGLE_THREAD ON)
set(IMAGE_EXPORT ON)

add_subdirectory(fmt)
# set(CMAKE_CXX_FLAGS "-O2")

if (IMAGE_EXPORT)
    message("Image mode.")
    set(MAIN_SOURCE src/image_main.cpp)
    set(SINGLE_THREAD ON)
else()
    set(MAIN_SOURCE src/main.cpp)
endif()

set(NIX_SOURCES
    src/nix/tempfile.cpp
    src/nix/fork.cpp
    src/nix/pipe.cpp
    src/nix/dynamic_loader.cpp
)

set(SOURCES
    ${NIX_SOURCES}
    src/rng.cpp
    src/plot/Graph.cpp
    src/Functions.cpp
    src/aux.cpp
    src/Precedence.cpp
    src/Lexer.cpp
    src/Parser.cpp
    ${MAIN_SOURCE}
)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(fmt)

include(CTest)
enable_testing()

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
    dl
)

if (SINGLE_THREAD)
    message("No Multithreading!")
    set_property(
        SOURCE ${MAIN_SOURCE}
        PROPERTY COMPILE_DEFINITIONS KHAT_SINGLETHREAD
    )
endif()

if (NOT SINGLE_THREAD)
    target_link_libraries(MathParser
        pthread
    )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
