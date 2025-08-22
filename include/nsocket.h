/**
 * \file
 * \brief NSocket class definition
 */
#include <stdint.h>

#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
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
      * \brief Verify if Socket is open
      * \return true if open, false otherwise
      */
     virtual bool isOpen() const {
        return (sock_fd_ != -1);
     }

     /**
      * \brief Create a Socket descriptor
      * \return true if created and false otherwise 
      */
     virtual bool CreateSocket() = 0;

     /**
      * \brief Send a message
      * \param data The message
      * \param ip_address the ip that should receive the message
      * \return The number of bytes sent, -1 for error
      */
     virtual int Send(const std::vector<uint8_t> &data, const std::string ip_address) const = 0;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> Receive(const int sbuffer) = 0;

     /**
      * \brief Send a message
      * \return true if closed and false otherwise
      */
     virtual bool Close() = 0;

     /**
      * \brief Set a new address
      * \param ip_address The desired ip address
      * \return true if successfull and false otherwise
      */
     virtual bool SetIP(const std::string ip_address) = 0;

     /**
      * \brief Decode an hostname or ip address
      * \param ip_address The ip address or hostname
      * \return The decoded ip address
      */
     virtual std::string DecodeAddress(const std::string ip_address) = 0;

     /**
      * \brief Get the source ip to achieve a desired destiny
      * \param ip_address The ip address or hostname to connect
      * \return The ip address of the source
      */
     virtual std::string GetSourceIP(const std::string ip_address) = 0;

     /**
      * \brief Get the type of socket
      * \return The socket type registered in attribute type
      */
     std::string type() const{
      return type_;
     }

     /**
      * \brief Set the last ip from a message were received
      * \param last_ip_com The last ip from a message were received
      */
     void set_last_ip_com(std::string last_ip_com){
      last_ip_com_ = last_ip_com;
     }

     /**
      * \brief Get the last ip from a message were received
      * \return The last ip from a message were received
      */
     virtual std::string last_ip_com() const{
      return last_ip_com_;
     }

     /**
      * \brief Constructor
      * \param version the version of the socket type
      */
     NSocket(std::string version = "") : type_(version){};

    private:
    std::string type_ = "";
    std::string last_ip_com_ = "";
    int sock_fd_ = -1;
};

 #endif

/*
@startuml

class NSocket {
    +sock_fd() const: int
    +set_sock_fd(int): void
    +CreateSocket(): bool
    +Send(const std::vector<uint8_t> &, const std::string) const: int
    +Receive(const int) const: std::vector<uint8_t>
    +Close(): bool
    +SetIP(const std::string): bool
    +DecodeAddress(const std::string): std::string
    +GetSourceIP(const std::string): std::string
    +type() const: std::string
    +set_last_ip_com(std::string): void
    +last_ip_com() const: std::string
    +NSocket(std::string)

    -type_: std::string
    -last_ip_com: std::string
    -sock_fd_: int
}

@enduml
*/