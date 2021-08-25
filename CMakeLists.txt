# CMakeLists.txt for Traceroute


cmake_minimum_required(VERSION 3.16)
set(This Traceroute)

set(Headers
    include/Traceroute/DataSenders/IcmpDataSender.hpp    
    include/Traceroute/DataSenders/TcpDataSender.hpp
    include/Traceroute/DataSenders/UdpDataSender.hpp    
    include/Traceroute/ResponseValidators/UdpResponseValidator.hpp
    include/Traceroute/ResponseValidators/IcmpResponseValidator.hpp
    include/Traceroute/ResponseValidators/TcpResponseValidator.hpp
    include/Traceroute/ProbeSender.hpp
    include/Traceroute/Packet.hpp
    include/Traceroute/Packet/IcmpPacket.hpp
    include/Traceroute/Packet/TcpPacket.hpp
    include/Traceroute/Packet/UdpPacket.hpp
    include/Traceroute/IPacketFactory.hpp
    include/Traceroute/PacketFactory/IcmpPacketFactory.hpp
    include/Traceroute/PacketFactory/TcpPacketFactory.hpp
    include/Traceroute/PacketFactory/UdpPacketFactory.hpp
    include/Traceroute/SocketAddress.hpp    
    include/Traceroute/IDataSender.hpp
    include/Traceroute/IValidateResponse.hpp
)

set(Sources
    src/DataSenders/IcmpDataSender.cpp
    src/DataSenders/UdpDataSender.cpp    
    src/DataSenders/TcpDataSender.cpp   
    src/ResponseValidators/IcmpResponseValidator.cpp
    src/ResponseValidators/V4/Icmp4ResponseValidator.hpp
    src/ResponseValidators/V4/Icmp4ResponseValidator.cpp
    src/ResponseValidators/V4/Utils.hpp
    src/ResponseValidators/V4/Utils.cpp
    src/ResponseValidators/V6/Icmp6ResponseValidator.hpp
    src/ResponseValidators/V6/Icmp6ResponseValidator.cpp   
    src/ResponseValidators/TcpResponseValidator.cpp
    src/ResponseValidators/UdpResponseValidator.cpp       
    src/ChecksumCalculator.cpp
    src/DataSenderBase.cpp
    src/Packet.cpp
    src/Packet/IcmpPacket.cpp
    src/Packet/TcpPacket.cpp
    src/Packet/UdpPacket.cpp
    src/PacketFactory/IcmpPacketFactory.cpp
    src/PacketFactory/TcpPacketFactory.cpp
    src/PacketFactory/UdpPacketFactory.cpp
    src/Probe.cpp
    src/ProbeSender.cpp
    src/SocketAddress.cpp
    src/PodSerializer.hpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)


target_include_directories(${This} PUBLIC include)

add_subdirectory(tests)
