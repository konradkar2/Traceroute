# CMakeLists.txt for StringExtensions

cmake_minimum_required(VERSION 3.8)
set(This TracerouteTests)

set(Sources
    src/SocketAddressTests.cpp
    src/LiveIcmpTest.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_include_directories(${This} PRIVATE ../src)

target_link_libraries(${This} PUBLIC
    gtest_main
    Traceroute
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
