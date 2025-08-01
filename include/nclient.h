/**
 * \file
 * \brief NClient class definition
 */
#include <stdint.h>

#include <string>
#include <vector>

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
      * \brief Constructor
      * \param socket_ptr Pointer to NSocket
      */
     NClient(std::shared_ptr<NSocket> socket_ptr = nullptr);

     /**
      * \brief Destructor
      */
     ~NClient();

     /**
      * \brief Get Socket descriptor
      * \return Socket descriptor
      */
     virtual int sock_fd() const{
        return socket_->sock_fd();
     }

     /**
      * \brief Verify is client is open
      * \return true if created and false otherwise
      */
     bool isOpen() const;

     /**
      * \brief Start a Ping process
      * \param ip_address The desired ip to achieve
      * \param times The number of echo requests to send
      * \return the log of the process
      */
     virtual std::string Ping(const std::string ip_address, const int times) const;

    private:
     std::shared_ptr<NSocket> socket_;
};

 #endif

/*
@startuml

class NClient {
    +NClient(std::shared_ptr<NSocket>)
    +~NClient();
    +sock_fd() const: int
    +isOpen() const: bool
    +Ping(const std::string, const int) const: std::string

    -socket_: std::shared_ptr<NSocket>
}

NClient 1-- NSocket

@enduml
*/