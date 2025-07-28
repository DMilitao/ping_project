/**
 * \file
 * \brief NSocket class definition
 */
#include "include/nsocket.h"

bool NSocket::CreateSocket(){
    if ( sock_fd_ != -1 ){
        return false;
    }

    sock_fd_ = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    struct timeval tv_recv;
    tv_recv.tv_sec = 1;
    tv_recv.tv_usec = 0;
    if ( setsockopt(sock_fd_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_recv, sizeof(tv_recv)) < 0 ) {
        Close();
        return false;
    }

    return sock_fd_ != -1;
}

bool NSocket::Close(){
    if ( sock_fd_ != -1 ){
        if ( close(sock_fd_) != -1 ) {
            sock_fd_ = -1;
            std::memset(&address_, 0, sizeof(address_));
            return true;
        }
    }
    return false;
}

int NSocket::Send(const std::vector<uint8_t> &data, const std::string ip_address) const {
    if (sock_fd_ == -1) {
        return -1;
    }

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;

    if ( inet_pton(AF_INET, ip_address.c_str(), &dest_addr.sin_addr) <= 0 ){
        return -1;
    }

    return sendto(sock_fd_, data.data(), data.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

int NSocket::Send(const std::vector<uint8_t> &data, const uint32_t ip_address) const {
    if (sock_fd_ == -1) {
        return -1;
    }

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;
    dest_addr.sin_addr.s_addr = htonl(ip_address);

    return sendto(sock_fd_, data.data(), data.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

std::vector<uint8_t> NSocket::Receive(const int sbuffer) const {
    std::vector<uint8_t> msg(sbuffer);

    sockaddr_in dest_addr;
    socklen_t dest_addr_len = sizeof(dest_addr);

    int read_bytes = recvfrom(sock_fd_, msg.data(), sbuffer, 0, (struct sockaddr*)&dest_addr, &dest_addr_len);

    if ( read_bytes == -1)
    {
       return {};
    }

    msg.resize(read_bytes);
    return msg;
}

bool NSocket::SetIP(const std::string ip_address){
    if (sock_fd_ == -1) {
        return false;
    }

    address_.sin_family = AF_INET;
    address_.sin_port = 0;

    if ( inet_pton(AF_INET, ip_address.c_str(), &address_.sin_addr) <= 0 ){
        Close();
        return false;
    }

    if ( bind(sock_fd_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        Close();
        return false;
    }

    return true;
}