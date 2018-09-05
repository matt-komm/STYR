#include_directories(${PROJECT_SOURCE_DIR}/include SYSTEM ${ROOT_INCLUDE_DIR} ${DELPHES_INCLUDE_DIR})

set(HEADER_FILES_FOR_DICT
    ${CMAKE_CURRENT_SOURCE_DIR}/MuonSelection.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ElectronSelection.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/JetSelection.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h
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


add_library(styr-plugins SHARED
    MuonSelection.cc
    ElectronSelection.cc
    JetSelection.cc
    dict.C
)
target_link_libraries(styr-plugins styr ${ROOT_LIBRARIES} ${DELPHES_LIBRARIES})

