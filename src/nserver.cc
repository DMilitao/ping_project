/**
 * \file
 * \brief NServer class definition
 */
#include "include/nserver.h"

bool NServer::StartServer(uint16_t port, uint32_t ip_address, int backlog){

    CreateSocket();

    if ( sock_fd() == -1 ){
        return false;
    }

    return SetIP(port, ip_address) && StartListen(backlog);
};

bool NServer::CloseServer(){
    return Close();
}