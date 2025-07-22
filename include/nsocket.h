/**
 * \file
 * \brief NSocket class definition
 */
#include <stdint.h>

#include <cstring>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef PING_PROJECT_INCLUDE_NSOCKET_H_
#define PING_PROJECT_INCLUDE_NSOCKET_H_

/**
 * \brief NSocket class
 */

class NSocket
{
    public:
     /**
      * \brief Get Socket descriptor
      * \return Socket descriptor
      */
     virtual int sock_fd() const{
        return sock_fd_;
     }

     /**
      * \brief Set Socket descriptor
      * \param sock_fd Socket descriptor
      */
     virtual void set_sock_fd(int sock_fd){
        sock_fd_ = sock_fd;
     }

     /**
      * \brief Get Address
      * \return Address
      */
     virtual inline sockaddr_in address() const{
        return address_;
     }

     /**
      * \brief Set Address
      * \param address Address
      */
     virtual inline void set_address(sockaddr_in address){
       address_ = address;
     }
     /**
      * \brief Create a Socket descriptor
      * \return true if created and false otherwise 
      */
     bool CreateSocket();

     /**
      * \brief Send a message
      * \param data The message
      * \return The number of bytes sent, -1 for error
      */
     virtual int Send(const std::vector<uint8_t> &data, uint32_t ip_address);

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> Receive(int sbuffer);

     /**
      * \brief Send a message
      * \return true if closed and false otherwise
      */
     virtual bool Close();

     /**
      * \brief Set a new address
      * \param ip_address The desired ip address
      * \return true if successfull and false otherwise
      */
     bool SetIP(uint32_t ip_address);

    private:
     int sock_fd_ = -1;
     sockaddr_in address_;
};

 #endif

/*
@startuml

class NSocket {
    +sock_fd() const: int
    +set_sock_fd(int): void
    +address() const: sockaddr_in
    +set_address(sockaddr_in): void
    +CreateSocket(): bool
    +Send(const std::vector<uint8_t> &): int
    +Receive(int): std::vector<uint8_t>
    +Close(): bool
    +SetIP(uint32_t): bool

    -sock_fd_: int
    -address_: sockaddr_in 
}

@enduml
*/