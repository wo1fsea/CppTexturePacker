cmake_minimum_required(VERSION 3.12)
set(PROJECT_PYBIND ${PROJECT_NAME}_pybind)
set(PY_MOUDLE_NAME cpptexturepacker)

find_package(pybind11 CONFIG REQUIRED)
find_package (Python COMPONENTS Development)

file(GLOB PROJECT_PYBIND_SOURCES *.cpp)

file(GLOB PROJECT_CONFIGS CMakeLists.txt)

source_group("include" FILES ${PROJECT_HEADERS})
source_group("src" FILES ${PROJECT_PYBIND_SOURCES})

include_directories(${Python_INCLUDE_DIRS})

pybind11_add_module(${PROJECT_PYBIND} ${PROJECT_PYBIND_SOURCES})

target_link_libraries(${PROJECT_PYBIND} PRIVATE ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_PYBIND} PRIVATE png) 
target_link_libraries(${PROJECT_PYBIND} PRIVATE ${PLIST_LIBRARY})
target_link_libraries(${PROJECT_PYBIND} PRIVATE Boost::filesystem)
target_link_libraries(${PROJECT_PYBIND} PRIVATE pybind11::embed pybind11::module pybind11::pybind11)
target_link_libraries(${PROJECT_PYBIND} PRIVATE ${Python_LIBRARIES})

if(NOT WIN32)
    target_link_libraries(${PROJECT_PYBIND} pthread)
endif()
