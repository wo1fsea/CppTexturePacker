cmake_minimum_required(VERSION 3.12)
project(cpptexturepacker)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

include_directories(include/
                   ${Boost_INCLUDE_DIRS})

file(GLOB PROJECT_HEADERS include/*.hpp
                          include/*.h)
file(GLOB PROJECT_SOURCES src/*.cpp)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

set(PROJECT_TEST_SOURCES ${PROJECT_SOURCES})
list(REMOVE_ITEM PROJECT_TEST_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

source_group("include" FILES ${PROJECT_HEADERS})
source_group("src" FILES ${PROJECT_SOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    )

# Unit tests
add_subdirectory(tests)