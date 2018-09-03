#include_directories(${PROJECT_SOURCE_DIR}/include SYSTEM ${ROOT_INCLUDE_DIR} ${DELPHES_INCLUDE_DIR})

set(HEADER_FILES_FOR_DICT
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


add_library(JetSelection SHARED
    JetSelection.cc
    dict.C
)
target_link_libraries(JetSelection styr ${ROOT_LIBRARIES} ${DELPHES_LIBRARIES})

