/**
 * \file
 * \brief NSocket class definition
 */
#include "include/nsocket.h"

#include <iostream>

bool NSocket::CreateSocket(){
    if ( sock_fd_ != -1 ){
        return false;
    }

    sock_fd_ = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    return sock_fd_ != -1 ? true : false;
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

int NSocket::Send(const std::vector<uint8_t> &data, uint32_t ip_address){
    if (sock_fd_ == -1) {
        return -1;
    }
    
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;
    dest_addr.sin_addr.s_addr = htonl(ip_address);

    return sendto(sock_fd_, data.data(), data.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

std::vector<uint8_t> NSocket::Receive(int sbuffer){
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

bool NSocket::SetIP(uint32_t ip_address){
    if (sock_fd_ == -1) {
        return false;
    }

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = htonl(ip_address);
    address_.sin_port = 0;

    if ( bind(sock_fd_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        Close();
        return false;
    }
    return true;
}