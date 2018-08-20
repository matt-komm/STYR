add_executable(testEvent
    testEvent.cc
)
target_link_libraries(testEvent styr)

add_executable(testModule
    testModule.cc
)
target_link_libraries(testModule styr)

add_executable(testBranch
    testBranch.cc
)
target_link_libraries(testBranch styr)

add_test(
    NAME testEvent
    COMMAND testEvent
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(
    NAME testModule
    COMMAND testModule
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(
    NAME testBranch
    COMMAND testBranch
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

