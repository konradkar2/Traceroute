# CMakeLists.txt for Traceroute


cmake_minimum_required(VERSION 3.16)
set(This Traceroute)

set(Headers
    include/Traceroute/SocketProviders/IcmpSocketProvider.hpp       
    include/Traceroute/SocketProviders/TcpSocketProvider.hpp       
    include/Traceroute/SocketProviders/UdpSocketProvider.hpp       
    include/Traceroute/ResponseValidators/UdpResponseValidator.hpp
    include/Traceroute/ResponseValidators/IcmpResponseValidator.hpp
    include/Traceroute/ResponseValidators/TcpResponseValidator.hpp
    include/Traceroute/ProbeSender.hpp
    include/Traceroute/Packet.hpp
    include/Traceroute/Packet/IcmpPacket.hpp
    include/Traceroute/Packet/TcpPacket.hpp
    include/Traceroute/Packet/UdpPacket.hpp
    include/Traceroute/IPacketFactory.hpp
    include/Traceroute/IProvideSocket.hpp
    include/Traceroute/PacketFactory/IcmpPacketFactory.hpp
    include/Traceroute/PacketFactory/TcpPacketFactory.hpp
    include/Traceroute/PacketFactory/UdpPacketFactory.hpp
    include/Traceroute/SocketAddress.hpp    
    include/Traceroute/ResponseValidators/ResponseValidator.hpp 
    include/Traceroute/IValidateResponse.hpp 
    include/Traceroute/DataSender.hpp
    include/Traceroute/Socket.hpp
)

set(Sources
    src/SocketProviders/IcmpSocketProvider.cpp
    src/SocketProviders/TcpSocketProvider.cpp
    src/SocketProviders/UdpSocketProvider.cpp
    src/ResponseValidators/IcmpResponseValidator.cpp
    src/ResponseValidators/ResponseValidator.cpp
    src/ResponseValidators/V4/Icmp4ResponseValidator.hpp
    src/ResponseValidators/V4/Icmp4ResponseValidator.cpp
    src/ResponseValidators/V4/Utils.hpp
    src/ResponseValidators/V4/Utils.cpp
    src/ResponseValidators/V6/Icmp6ResponseValidator.hpp
    src/ResponseValidators/V6/Icmp6ResponseValidator.cpp   
    src/ResponseValidators/TcpResponseValidator.cpp
    src/ResponseValidators/UdpResponseValidator.cpp       
   
    src/DataSender.cpp
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
    src/utils/ChecksumCalculator.cpp
    src/utils/Serialize.hpp
    src/utils/Poll.cpp
    src/utils/CreateSocket.cpp
    src/ResponseValidators/common.hpp
    src/ResponseValidators/common.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)


target_include_directories(${This} PUBLIC include)
target_include_directories(${This} PRIVATE src)

add_subdirectory(tests)
