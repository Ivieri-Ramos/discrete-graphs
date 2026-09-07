cmake_minimum_required(VERSION 3.15...3.27)
project(discrete_graphs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2 /W4)
else()
    add_compile_options(-O3 -Wall -Wextra)
endif()

find_package(pybind11 CONFIG REQUIRED)

set(SOURCES
    csrc/Graphs.cpp
    csrc/TarjanSolver.cpp
    csrc/ChromaticSolver.cpp
    csrc/pybind_module.cpp
)

pybind11_add_module(graph_ext ${SOURCES})

target_include_directories(graph_ext PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/csrc
)
target_link_libraries(graph_ext PRIVATE stdc++)

install(TARGETS graph_ext DESTINATION .)