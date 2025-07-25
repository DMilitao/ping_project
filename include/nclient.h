/**
 * \file
 * \brief NClient class definition
 */
#include <stdint.h>

#include <string>
#include <sstream>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_NCLIENT_H_
#define PING_PROJECT_INCLUDE_NCLIENT_H_

/**
 * \brief NClient class
 */

class NClient
{
    public:
     /**
      * \brief Get Socket descriptor
      * \return Socket descriptor
      */
     virtual int sock_fd() const{
        return socket_.sock_fd();
     }

     /**
      * \brief Start a new client object
      * \return true if created and false otherwise 
      */
     bool Open(const std::string ip_address);

     /**
      * \brief Verify is client is open
      * \return true if created and false otherwise 
      */
     bool isOpen() const;

     /**
      * \brief Try to disconnect from a server
      * \return true if closed and false otherwise
      */
     bool Close();

     /**
      * \brief Send a message
      * \param data The message
      * \param ip_address The ip that should receive the message
      * \return The number of bytes sent, -1 for error
      */
     virtual int Send(const std::vector<uint8_t> &data, const std::string ip_address) const;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> WaitingMessage(const int sbuffer) const;

     /**
      * \brief Start a Ping process
      * \param ip_address The desired ip to achieve
      * \return the log of the process
      */
     std::string Ping(const std::string ip_address) const;

    private:
     NSocket socket_;
};

 #endif

/*
@startuml

class NClient {
    +Open(const std::string): bool
    +isOpen() const: bool
    +Close(): bool
    +Send(const std::vector<uint8_t> &, const std::string) const: int
    +WaitingMessage(const int) const: bool
    +Ping(const std::string) const: std::string

    -socket_: NSocket
}

@enduml
*/