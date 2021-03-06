cmake_minimum_required(VERSION 3.12)
project(cpptexturepacker)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(CImg CONFIG REQUIRED)
find_package(JPEG REQUIRED)
find_package(libpng CONFIG REQUIRED)

find_path(UTFCPP_INCLUDE_DIR utf8/core.h)
find_path(PLIST_INCLUDE_DIR plist/plist.h)

set(plist_names ${PLIST_NAMES} plist plist-static libplist libplist-static)

if(NOT PLIST_LIBRARY)
  find_library(PLIST_LIBRARY NAMES ${plist_names})
endif()
unset(plist_names)

if(APPLE)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
  set(PNG_LIBRATY png_static)
else()
  set(PNG_LIBRATY png)
endif()

include_directories(include/
                   ${Boost_INCLUDE_DIRS}
                   ${CIMG_INCLUDE_DIR}
                   ${JPEG_INCLUDE_DIR}
                   ${PNG_INCLUDE_DIR}
                   ${PUGIXML_INCLUDE_DIR}
                   ${UTFCPP_INCLUDE_DIR}
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

target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRATY}) 
target_link_libraries(${PROJECT_NAME} ${PLIST_LIBRARY})
target_link_libraries(${PROJECT_NAME} Boost::filesystem)

if(NOT WIN32)
  target_link_libraries(${PROJECT_NAME} pthread)
endif()

set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    )

add_subdirectory(pybind)

get_filename_component(DATA_PATH
                       data/
                       ABSOLUTE)
add_compile_definitions(TEST_DATA_PATH="${DATA_PATH}")
# Unit tests
enable_testing()
add_subdirectory(tests)