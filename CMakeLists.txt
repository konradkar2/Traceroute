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
    src/Icmp/IcmpResponseValidator.cpp
    src/Icmp/V4/Icmp4ResponseValidator.hpp
    src/Icmp/V4/Icmp4ResponseValidator.cpp
    src/Icmp/V6/Icmp6ResponseValidator.hpp
    src/Icmp/V6/Icmp6ResponseValidator.cpp

    src/Tcp/TcpDataSender.cpp
   
    src/Tcp/TcpResponseValidator.cpp
    src/Udp/UdpDataSender.cpp
   
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


target_include_directories(${This} PUBLIC include)

add_subdirectory(tests)
