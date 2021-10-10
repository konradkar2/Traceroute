# CMakeLists.txt for Traceroute

cmake_minimum_required(VERSION 3.16)
set(This UnitTests)

set(Headers 

    ResponseValidatorsTests/Responses.hpp
    ResponseValidatorsTests/V4/IpHeaderBuilder.hpp
    ResponseValidatorsTests/V4/IcmpRequest/IcmpToIcmpBase.hpp
)

set(Sources
    SocketAddressTests.cpp
    ResponseValidatorsTests/V4/IcmpRequest/TimeExceeded.cpp
    ResponseValidatorsTests/V4/IcmpRequest/EchoReply.cpp
)

add_executable(${This} ${Sources} ${Header})
set_target_properties(${This} PROPERTIES
    FOLDER Tests/UnitTests
)

target_link_libraries(${This} PUBLIC
    gtest_main
    gmock
    Traceroute
)

target_include_directories(${This} PRIVATE ../../src)
target_include_directories(${This} PRIVATE ${CMAKE_CURRENT_LIST_DIR})


add_test(
    NAME ${This}
    COMMAND ${This}
)
