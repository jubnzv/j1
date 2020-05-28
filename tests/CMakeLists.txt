set(proj_src
    ${PROJECT_SOURCE_DIR}/src/VM.cpp ${PROJECT_SOURCE_DIR}/src/Stack.cpp
    ${PROJECT_SOURCE_DIR}/src/J1.cpp ${PROJECT_SOURCE_DIR}/src/IOConsole.cpp)

add_executable(unit_tests ${proj_src} TestVM.cpp TestStack.cpp)

set_property(TARGET unit_tests PROPERTY CXX_STANDARD 20)

target_link_libraries(unit_tests gtest_main)

add_test(NAME unit
         COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src)
