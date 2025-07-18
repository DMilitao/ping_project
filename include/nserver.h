/**
 * \file
 * \brief NServer class definition
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

#ifndef PING_PROJECT_INCLUDE_NSERVER_H_
#define PING_PROJECT_INCLUDE_NSERVER_H_

/**
 * \brief NServer class
 */

class NServer : public NSocket
{
    public:
     /**
      * \brief StartServer
      * \return true if created and false otherwise 
      */
     bool StartServer(uint16_t port, uint32_t ip_address = INADDR_ANY, int backlog = 10);
     
     /**
      * \brief Send a message
      * \return true if closed and false otherwise
      */
     
      bool CloseServer();
};

 #endif

/*
@startuml

class NSocket {
    +StartServer(uint16_t, uint32_t, int): bool
    +CloseServer(): bool
}

@enduml
*/