cmake_minimum_required (VERSION 3.0)
project (STYR)
add_definitions(-std=c++0x)
add_definitions(-Wall)
add_definitions(-Wextra)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(ROOT REQUIRED)
find_package(Delphes REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} SYSTEM ${ROOT_INCLUDE_DIR} ${DELPHES_INCLUDE_DIR})

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dict.C 
    COMMAND ${ROOT_DICT_EXECUTABLE} 
        -f ${CMAKE_CURRENT_BINARY_DIR}/dict.C 
        -c ${CMAKE_CURRENT_SOURCE_DIR}/Process.h  
        ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "generating dictionaries"
)

add_library(styr MODULE
    Process.cc
    dict.C
)

target_link_libraries(styr ${ROOT_LIBRARIES} ${DELPHES_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_CURRENT_BINARY_DIR}/test.py COPYONLY)

