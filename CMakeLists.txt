cmake_minimum_required(VERSION 3.12)
project(cpptexturepacker)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
find_package(CImg CONFIG REQUIRED)
find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)

find_path(PLIST_INCLUDE_DIR plist/plist.h)

set(plist_names ${PLIST_NAMES} plist plist-static libplist libplist-static)

if(NOT PLIST_LIBRARY)
  find_library(PLIST_LIBRARY NAMES ${plist_names})
endif()
unset(plist_names)

if (APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

include_directories(include/
                   ${Boost_INCLUDE_DIRS}
                   ${CIMG_INCLUDE_DIR}
                   ${JPEG_INCLUDE_DIR}
                   ${PNG_INCLUDE_DIR}
                   ${PUGIXML_INCLUDE_DIR}
                   ${PLIST_INCLUDE_DIR}
                   )

file(GLOB PROJECT_HEADERS include/*.hpp
                          include/*.h
                          )
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
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} ${PLIST_LIBRARY})

set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    )


# Unit tests
enable_testing()
add_subdirectory(tests)