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

    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);

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

int NSocket::Send(const std::vector<uint8_t> &data){
    if (sock_fd_ == -1) {
        return -1;
    }

    return send(sock_fd_, data.data(), data.size(), 0);
}

std::vector<uint8_t> NSocket::Receive(int sbuffer){
    std::vector<uint8_t> msg(sbuffer);

    int read_bytes = read(sock_fd_, msg.data(), sbuffer);

    if ( read_bytes == -1)
    {
       return {};
    }
    
    msg.resize(read_bytes);
    return msg;
}

bool NSocket::SetIP(uint16_t port, uint32_t ip_address){
    if (sock_fd_ == -1) {
        return false;
    }

    address_.sin_family = AF_INET;
    address_.sin_addr.s_addr = ip_address;
    address_.sin_port = htons(port);

    if ( bind(sock_fd_, (struct sockaddr *)&address_, sizeof(address_)) == -1) {
        Close();
        return false;
    }
    return true;
}

bool NSocket::StartListen(int backlog){
    if (sock_fd_ == -1) {
        return false;
    }

    if ( listen(sock_fd_, backlog) == -1) {
        Close();
        return false;
    }
    return true;
}

std::unique_ptr<NSocket> NSocket::AcceptConnection(){  
    if (sock_fd_ == -1) {
        return nullptr;
    }
    
    sockaddr_in new_connect;

    int new_connect_fd = accept(sock_fd_, (struct sockaddr *)&new_connect, (socklen_t *)(sizeof(new_connect)));
    
    if (new_connect_fd == -1) {
        return nullptr;
    }

    std::unique_ptr<NSocket> new_socket_obj = std::make_unique<NSocket>();
    new_socket_obj->set_sock_fd(new_connect_fd);
    new_socket_obj->set_address(new_connect);

    return new_socket_obj;
}

bool NSocket::NewConnection(uint16_t port, uint32_t ip_address){
    
    if (sock_fd_ == -1) {
        return false;
    }

    sockaddr_in new_connect;
    
    new_connect.sin_addr.s_addr = ip_address;
    new_connect.sin_family = AF_INET;
    new_connect.sin_port = htons(port);

    int new_connect_fd = connect(sock_fd_, (struct sockaddr *)&new_connect, (sizeof(new_connect)));
    
    return new_connect_fd == -1 ? false : true;

};