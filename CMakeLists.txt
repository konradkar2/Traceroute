# CMakeLists.txt for Traceroute


cmake_minimum_required(VERSION 3.16)
set(This Traceroute)

set(Headers
    include/Traceroute/Icmp/IcmpDataSender.hpp
    include/Traceroute/Icmp/IcmpResponseValidator.hpp
    include/Traceroute/Tcp/TcpDataSender.hpp
    include/Traceroute/Tcp/TcpResponseValidator.hpp
    include/Traceroute/Udp/UdpDataSender.hpp
    include/Traceroute/Udp/UdpResponseValidator.hpp

    include/Traceroute/ProbeSender.hpp
    include/Traceroute/PacketBuilder.hpp
    include/Traceroute/SocketAddress.hpp    
    include/Traceroute/IDataSender.hpp
    include/Traceroute/IValidateResponse.hpp
)

set(Sources
    src/Icmp/IcmpDataSender.cpp
    #src/Icmp/IcmpProbeSender.cpp
    src/Icmp/IcmpResponseValidator.cpp
    src/Tcp/TcpDataSender.cpp
    #src/Tcp/TcpProbeSender.cpp
    src/Tcp/TcpResponseValidator.cpp
    src/Udp/UdpDataSender.cpp
    #src/Udp/UdpProbeSender.cpp
    src/Udp/UdpResponseValidator.cpp
    src/ChecksumCalculator.cpp
    
    src/NBlockDataSenderBase.cpp
    src/PacketBuilder.cpp
    src/Packet.cpp
    src/Probe.cpp
    src/ProbeSender.cpp
    src/SocketAddress.cpp
    src/PodSerializer.hh
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)
target_include_directories(${This} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${This}>
    $<INSTALL_INTERFACE:include/${This}>
)

target_include_directories(${This} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${This}/Icmp>
    $<INSTALL_INTERFACE:include/${This}/Icmp>
)

target_include_directories(${This} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${This}/Tcp>
    $<INSTALL_INTERFACE:include/${This}/Tcp>
)

target_include_directories(${This} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${This}/Udp>
    $<INSTALL_INTERFACE:include/${This}/Udp>
)
target_include_directories(${This} PUBLIC include)

add_subdirectory(tests)
