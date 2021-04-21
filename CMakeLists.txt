cmake_minimum_required(VERSION 3.0.0)
project(MathParser VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/SelbaWard/Line.cpp
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

add_executable(MathParser ${SOURCES})
target_include_directories(MathParser
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(MathParser
    sfml-graphics
    sfml-window
    sfml-system
    pthread
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)