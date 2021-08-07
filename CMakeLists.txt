# CMakeLists.txt for Traceroute


cmake_minimum_required(VERSION 3.16)
set(This Traceroute)

set(Headers
    include/Traceroute/ChecksumCalculator.hpp
    include/Traceroute/HeaderTypes.hpp
    include/Traceroute/IcmpDataSender.hpp
    include/Traceroute/IcmpProbeSender.hpp
    include/Traceroute/NBlockDataSenderBase.hpp
    include/Traceroute/PacketBuilder.hpp
    include/Traceroute/Packet.hpp
    include/Traceroute/Probe.hpp
    include/Traceroute/ProbeSenderBase.hpp
    include/Traceroute/Serializable.hpp
    include/Traceroute/SocketAddress.hpp
    include/Traceroute/TcpDataSender.hpp
    include/Traceroute/TcpProbeSender.hpp
    include/Traceroute/UdpDataSender.hpp
    include/Traceroute/UdpProbeSender.hpp
    include/Traceroute/IDataSender.hpp
)

set(Sources
    src/ChecksumCalculator.cpp
    src/IcmpDataSender.cpp
    src/IcmpProbeSender.cpp
    src/NBlockDataSenderBase.cpp
    src/PacketBuilder.cpp
    src/Packet.cpp
    src/Probe.cpp
    src/ProbeSenderBase.cpp
    src/SocketAddress.cpp
    src/TcpDataSender.cpp
    src/TcpProbeSender.cpp
    src/UdpDataSender.cpp
    src/UdpProbeSender.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${This} PUBLIC include)

add_subdirectory(tests)
