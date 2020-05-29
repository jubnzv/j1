set(proj_src
    ${PROJECT_SOURCE_DIR}/src/vm/VM.cpp ${PROJECT_SOURCE_DIR}/src/vm/Stack.cpp
    ${PROJECT_SOURCE_DIR}/src/vm/J1.cpp
    ${PROJECT_SOURCE_DIR}/src/vm/IOConsole.cpp)

add_executable(unit_tests ${proj_src} TestVM.cpp TestStack.cpp)

set_property(TARGET unit_tests PROPERTY CXX_STANDARD 20)

target_link_libraries(unit_tests gtest_main)

message(${CMAKE_BINARY_DIR})
add_test(
  NAME unit
  COMMAND ${CMAKE_BINARY_DIR}/tests/output/${CMAKE_INSTALL_BINDIR}/unit_tests)

target_include_directories(unit_tests PUBLIC ${PROJECT_SOURCE_DIR}/src/vm)
include_directories(${PROJECT_SOURCE_DIR}/src/lib/)
