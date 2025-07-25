/**
 * \file
 * \brief NServer class definition
 */
#include <stdint.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_NSERVER_H_
#define PING_PROJECT_INCLUDE_NSERVER_H_

/**
 * \brief NServer class
 */

class NServer
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
      * \brief Start the server
      * \param ip_address The ip address where the server will listen
      * \return true if created and false otherwise 
      */
     bool Open(const std::string ip_address);

     /**
      * \brief Verify is server is started
      * \return true if created and false otherwise 
      */
     bool isOpen() const;

     /**
      * \brief Close server
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
      * \brief Send a message
      * \param data The message
      * \param ip_address The ip that should receive the message
      * \return The number of bytes sent, -1 for error
      */
     virtual int Send(const std::vector<uint8_t> &data, const uint32_t ip_address) const;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> WaitingMessage(const int sbuffer) const;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
      virtual int HandleMessage(const std::vector<uint8_t> &msg) const;

    private:
     NSocket socket_;
};

 #endif

/*
@startuml

class NServer {
    +Open(const std::string): bool
    +isOpen() const: bool
    +Close(): bool
    +Send(const std::vector<uint8_t> &, const std::string) const: int
    +Send(const std::vector<uint8_t> &, const uint32_t) const: int
    +WaitingMessage(const int) const: bool

   -socket_: NSocket

}

@enduml
*/