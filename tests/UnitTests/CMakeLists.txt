# CMakeLists.txt for Traceroute

cmake_minimum_required(VERSION 3.16)
set(This UnitTests)

set(Headers 
    ResponseValidatorsTests/utils/Utils.hpp
    ResponseValidatorsTests/utils/IpHeaderBuilder.hpp
    ResponseValidatorsTests/IcmpRequest/IcmpToIcmpBase.hpp
)

set(Sources
    SocketAddressTests.cpp
    ResponseValidatorsTests/IcmpRequest/TransitRouterResponseTest.cpp
    ResponseValidatorsTests/IcmpRequest/TargetResponseTest.cpp
    ResponseValidatorsTests/utils/Utils.cpp
    ResponseValidatorsTests/utils/ResponseBuilderTests.cpp
    
)

add_executable(${This} ${Sources} ${Header})
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
