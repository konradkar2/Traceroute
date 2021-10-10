# CMakeLists.txt for Traceroute

cmake_minimum_required(VERSION 3.16)
set(This EndToEndTestsRootRequired)

set(Sources
    src/LiveIcmpTest.cpp
    src/LiveTcpTest.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests/EndToEndTestsRootRequired
)

target_include_directories(${This} PRIVATE ../src)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock
    Traceroute
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
