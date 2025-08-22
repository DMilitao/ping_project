/**
 * \file
 * \brief NSocket IPv6 class definition
 */
#include "include/nsocket_ipv6.h"

bool NSocketIPv6::CreateSocket(){
    if ( isOpen() ){
        return false;
    }

    set_sock_fd(socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6));

    struct timeval tv_recv;
    tv_recv.tv_sec = 1;
    tv_recv.tv_usec = 0;
    if ( setsockopt(sock_fd(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_recv, sizeof(tv_recv)) < 0 ) {
        Close();
        return false;
    }

    return sock_fd() != -1;
}

bool NSocketIPv6::Close(){
    if ( isOpen() ){
        if ( close(sock_fd()) != -1 ) {
            set_sock_fd(-1);
            std::memset(&address_, 0, sizeof(address_));
            return true;
        }
    }
    return false;
}

int NSocketIPv6::Send(const std::vector<uint8_t> &data, const std::string ip_address) const {
    if ( !isOpen() ) {
        return -1;
    }

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(ip_address.c_str(), NULL, &hints, &result) != 0){
        return -1;
    }

    return sendto(sock_fd(), data.data(), data.size(), 0, result->ai_addr, result->ai_addrlen);
}

std::vector<uint8_t> NSocketIPv6::Receive(const int sbuffer) {
    std::vector<uint8_t> msg(sbuffer);

    sockaddr_in6 dest_addr;
    socklen_t dest_addr_len = sizeof(dest_addr);

    int read_bytes = recvfrom(sock_fd(), msg.data(), sbuffer, 0, (struct sockaddr*)&dest_addr, &dest_addr_len);

    if ( read_bytes == -1)
    {
       set_last_ip_com("");
       return {};
    }

    char ip_str[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6, &(dest_addr.sin6_addr), ip_str, sizeof(ip_str));

    set_last_ip_com(std::string(ip_str));

    msg.resize(read_bytes);
    return msg;
}

bool NSocketIPv6::SetIP(const std::string ip_address){
    if ( !isOpen() ) {
        return false;
    }

    address_.sin6_family = AF_INET6;
    address_.sin6_port = 0;

    if ( inet_pton(AF_INET6, DecodeAddress(ip_address).c_str(), &address_.sin6_addr) <= 0 ){
        Close();
        return false;
    }

    if ( bind(sock_fd(), (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        Close();
        return false; 
    }

    return true;
}

std::string NSocketIPv6::DecodeAddress(const std::string ip_address){
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(ip_address.c_str(), NULL, &hints, &result) != 0){
        return "";
    }

    struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)result->ai_addr;

    char ip_str[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip_str, sizeof(ip_str));

    return std::string(ip_str);
}

std::string NSocketIPv6::GetSourceIP(const std::string ip_address) {
    int sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock == -1) {
        return "";
    }

    struct sockaddr_in6 dest_addr = {};
    dest_addr.sin6_family = AF_INET6;
    dest_addr.sin6_port = 0;
    if (inet_pton(AF_INET6, DecodeAddress(ip_address).c_str(), &dest_addr.sin6_addr) != 1) {
        return "";
    }

    if (connect(sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) {
        close(sock);
        return "";
    }

    struct sockaddr_storage local_addr;
    socklen_t addr_len = sizeof(local_addr);
    if (getsockname(sock, (struct sockaddr*)&local_addr, &addr_len) == -1) {
        close(sock);
        return "";
    }

    struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)(&local_addr);

    char ip_str[INET6_ADDRSTRLEN];

    inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip_str, sizeof(ip_str));
    close(sock);

    return std::string(ip_str);
}