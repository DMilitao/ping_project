/**
 * \file
 * \brief NServer class definition
 */
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
      * \brief Constructor
      * \param ip_address The desired ip address where the server will listen
      * \param socket_ptr Pointer to NSocket
      */
     NServer(const std::string ip_address, std::shared_ptr<NSocket> socket_ptr = nullptr);

     /**
      * \brief Destructor
      */
     ~NServer();

     /**
      * \brief Get Socket descriptor
      * \return Socket descriptor
      */
     virtual int sock_fd() const{
        return socket_->sock_fd();
     }

     /**
      * \brief Verify is server is started
      * \return true if created and false otherwise
      */
     bool isOpen() const;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> Receive(const int sbuffer) const;

     /**
      * \brief Handle a received message
      * \param sbuffer The received message
      * \return The log of the process
      */
      virtual std::string HandleMessage(const std::vector<uint8_t> &msg) const;

    private:
      std::shared_ptr<NSocket> socket_;
};

 #endif

/*
@startuml

class NServer {
    +NServer(const std::string, std::shared_ptr<NSocket>);
    +~NServer();
    +sock_fd() const: int
    +isOpen() const: bool
    +Receive(const int) const: bool
    +HandleMessage(const std::vector<uint8_t>) const: std::string;

   -socket_: std::shared_ptr<NSocket>
}

NServer 1-- NSocket

@enduml
*/