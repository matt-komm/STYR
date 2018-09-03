cmake_minimum_required (VERSION 3.0)
project (STYR)
add_definitions(-std=c++0x)
add_definitions(-Wall)
add_definitions(-Wextra)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
find_package(ROOT REQUIRED)
find_package(Delphes REQUIRED)
set(STYR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${STYR_INCLUDE_DIR} SYSTEM ${ROOT_INCLUDE_DIR} ${DELPHES_INCLUDE_DIR})

set(HEADER_FILES_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/include/styr/Process.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/include/styr/Event.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/include/styr/Module.h  
    ${CMAKE_CURRENT_SOURCE_DIR}/include/styr/LinkDef.h
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dict.C 
    COMMAND ${ROOT_DICT_EXECUTABLE} 
        -f ${CMAKE_CURRENT_BINARY_DIR}/dict.C 
        -I${STYR_INCLUDE_DIR}
        -I${DELPHES_INCLUDE_DIR}
        -c ${HEADER_FILES_FOR_DICT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "generating dictionaries"
)

add_library(styr SHARED
    src/Process.cc
    src/Event.cc
    dict.C
)

target_link_libraries(styr ${ROOT_LIBRARIES} ${DELPHES_LIBRARIES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test.py ${CMAKE_CURRENT_BINARY_DIR}/test.py COPYONLY)


OPTION(ENABLE_TEST "Build tests" ON)
if (ENABLE_TEST)
    enable_testing()
    # add exta warnings
    if (OSNAME STREQUAL "LINUX")
        set(CMAKE_CXX_FLAGS "-Wextra -Wall -pedantic -Wno-unused-parameter ${CMAKE_CXX_FLAGS}" )
    endif (OSNAME STREQUAL "LINUX")
    if (OSNAME STREQUAL "WINDOWS")
        set(CMAKE_CXX_FLAGS "/Wall ${CMAKE_CXX_FLAGS}" )
    endif (OSNAME STREQUAL "WINDOWS")
    
    add_subdirectory(test)
endif (ENABLE_TEST)


add_subdirectory(plugins)


