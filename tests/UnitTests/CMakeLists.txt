# CMakeLists.txt for Traceroute

cmake_minimum_required(VERSION 3.16)
set(This UnitTests)

set(Sources
    SocketAddressTests.cpp
    ResponseValidatorsTests/IcmpRequest/TimeExceededV4.cpp
    ResponseValidatorsTests/IcmpRequest/EchoReplyV4.cpp
    ResponseValidatorsTests/TcpRequest/TcpTimeExceededV4.cpp
    ResponseValidatorsTests/TcpRequest/TcpTimeExceededV6.cpp
    ResponseValidatorsTests/TcpRequest/TcpAckV4.cpp
    ResponseValidatorsTests/TcpRequest/TcpAckV6.cpp
    ResponseValidatorsTests/TcpRequest/TcpDestUnreachV4.cpp
    ResponseValidatorsTests/TcpRequest/TcpDestUnreachV6.cpp
    ResponseValidatorsTests/IcmpRequest/EchoReplyV6.cpp
    ResponseValidatorsTests/IcmpRequest/TimeExceededV6.cpp
    ResponseValidatorsTests/UdpRequest/UdpDestUnreachV4.cpp
    ResponseValidatorsTests/UdpRequest/UdpDestUnreachV6.cpp
    ResponseValidatorsTests/UdpRequest/UdpTimeExceededV4.cpp
    ResponseValidatorsTests/UdpRequest/UdpTimeExceededV6.cpp

    ProbeSenderTests/ProbeSenderTest.cpp
)

add_executable(${This} ${Sources} )
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
