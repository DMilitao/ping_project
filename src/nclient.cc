/**
 * \file
 * \brief NClient class definition
 */
#include "include/nclient.h"

bool NClient::ConnectToServer(uint16_t port, uint32_t ip_address){

    CreateSocket();

    if ( sock_fd() == -1 ){
        return false;
    }

    return NewConnection(port, ip_address);
};

bool NClient::DisconnectFromServer(){
    return Close();
}