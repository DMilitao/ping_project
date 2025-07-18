/**
 * \file
 * \brief NClient class definition
 */
#include <stdint.h>

#include <string>
#include <stdexcept>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_NCLIENT_H_
#define PING_PROJECT_INCLUDE_NCLIENT_H_

/**
 * \brief NSocket class
 */

class NClient : public NSocket
{
    public:
     /**
      * \brief Try new connection to server
      * \return true if created and false otherwise 
      */
     virtual bool ConnectToServer(uint16_t port, uint32_t ip_address);
     
     /**
      * \brief Try to disconnect from a server
      * \return true if closed and false otherwise
      */
      bool DisconnectFromServer();
};

 #endif

/*
@startuml

class NSocket {
    +ConnectToServer(uint16_t, uint32_t): bool
    +DisconnectFromServer(): bool
}

@enduml
*/