cmake_minimum_required(VERSION 3.12)
set(PROJECT_UNIT_TESTS_NAME ${PROJECT_NAME}_unit_test)

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if (APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
endif()

# add_compile_definitions(BOOST_TEST_MAIN)
include_directories(include/
                   ${Boost_INCLUDE_DIRS})

file(GLOB PROJECT_TESTS *.cpp)

file(GLOB PROJECT_CONFIGS CMakeLists.txt)

source_group("include" FILES ${PROJECT_HEADERS})
source_group("src" FILES ${PROJECT_TEST_SOURCES} ${PROJECT_TESTS})

add_executable(${PROJECT_UNIT_TESTS_NAME} ${PROJECT_HEADERS} ${PROJECT_TEST_SOURCES} ${PROJECT_TESTS})

target_link_libraries(${PROJECT_UNIT_TESTS_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(${PROJECT_UNIT_TESTS_NAME} ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_UNIT_TESTS_NAME} png) 
target_link_libraries(${PROJECT_UNIT_TESTS_NAME} ${PLIST_LIBRARY})
target_link_libraries(${PROJECT_UNIT_TESTS_NAME} Boost::filesystem)

if(NOT WIN32)
  target_link_libraries(${PROJECT_NAME} pthread)
endif()

set_target_properties(
    ${PROJECT_UNIT_TESTS_NAME} 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_UNIT_TESTS_NAME}
    )

add_test(NAME test COMMAND ${PROJECT_UNIT_TESTS_NAME})
