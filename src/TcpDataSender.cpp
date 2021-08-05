#include <Traceroute/TcpDataSender.hpp>


namespace Traceroute
{
    TcpDataSender::TcpDataSender(int family, const SocketAddress & sourceAddr,int delayMs)
        : NBlockDataSenderBase(family,sourceAddr,delayMs)
    {
        _sfd_tcp = socket(_sock_family, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_TCP);        
        
        if((bind(_sfd_tcp,SA sourceAddr.GetAddress(),sourceAddr.GetSize())) < 0)
        {
            throw std::runtime_error("Could not bind address");
        }
        if(_sock_family == AF_INET6)
        {
            int offset = 16;
            if((setsockopt(_sfd_tcp, IPPROTO_IPV6, IPV6_CHECKSUM,
                &offset, sizeof(offset))) < 0)
            {
                throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
            }

        }
        _sfd_current = _sfd_icmp;
    }
    
    int TcpDataSender::GetCurrentProtocol()
    {
        if(_sfd_current == _sfd_tcp)
            return IPPROTO_TCP;
        else
        {
            int temp = _sock_family == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6;
            return temp;
        }
        return -1;
    }
    int TcpDataSender::GetSendingSocket()
    {
       return _sfd_tcp;
    }

    int TcpDataSender::GetReceivingSocket()
    {
        int temp = _sfd_current;

        //swap current receiving socket 
        _sfd_current = (_sfd_current == _sfd_icmp ) ? _sfd_tcp : _sfd_icmp;

        return temp;
    }

}