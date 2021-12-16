# CMakeLists.txt for Traceroute


cmake_minimum_required(VERSION 3.16)
set(This Traceroute)

set(Sources
    src/SocketProviders/IcmpSocketProvider.cpp
    src/SocketProviders/TcpSocketProvider.cpp
    src/SocketProviders/UdpSocketProvider.cpp
    src/ResponseValidators/ResponseValidator.cpp
    src/ResponseValidators/IcmpResponseValidator.cpp
    src/ResponseValidators/TcpResponseValidator.cpp
    src/ResponseValidators/TcpToTcpResponseValidator.cpp
    src/ResponseValidators/IcmpToTcpResponseValidator.cpp
    src/ResponseValidators/UdpResponseValidator.cpp
    src/ResolveSourceAddress.cpp
    src/DataSender.cpp
    src/Packet.cpp
    src/Packet/IcmpPacket.cpp
    src/Packet/TcpPacket.cpp
    src/Packet/UdpPacket.cpp
    src/ResponseInfo.cpp
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
    src/utils/Bind.cpp
    src/ResponseValidators/common.hpp
    src/ResponseValidators/common.cpp
    src/SystemClock.cpp
)

add_library(${This} STATIC ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)


target_include_directories(${This} PUBLIC include)
target_include_directories(${This} PRIVATE src)

add_subdirectory(tests)
