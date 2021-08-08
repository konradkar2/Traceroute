# CMakeLists.txt for Traceroute

cmake_minimum_required(VERSION 3.16)
set(This UnitTests)

set(Sources
    src/SocketAddressTests.cpp
    src/ResponseValidatorsTests/Icmp4ResponseValidatorTests.cpp
    src/ResponseValidatorsTests/Utils.hpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests/UnitTests
)

target_link_libraries(${This} PUBLIC
    gtest_main
    Traceroute
)
target_include_directories(${This} PRIVATE ../../src)
add_test(
    NAME ${This}
    COMMAND ${This}
)
