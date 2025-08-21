/**
 * \file
 * \brief NSocket IPv6 class definition
 */
#include <stdint.h>

#include <netdb.h>

#include "nsocket.h"

#ifndef PING_PROJECT_INCLUDE_NSOCKET_IPV6_H_
#define PING_PROJECT_INCLUDE_NSOCKET_IPV6_H_

/**
 * \brief NSocket IPv6 class
 */

class NSocketIPv6 : public NSocket
{
    public:
     /**
      * \brief Get Address
      * \return Address
      */
     virtual inline sockaddr_in6 address() const{
        return address_;
     }

     /**
      * \brief Set Address
      * \param address Address
      */
     virtual inline void set_address(sockaddr_in6 address){
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
      * \param ip_address the ip that should receive the message
      * \return The number of bytes sent, -1 for error
      */
     virtual int Send(const std::vector<uint8_t> &data, const std::string ip_address) const;

     /**
      * \brief Read a message
      * \param sbuffer The expected size of the message
      * \return The received message
      */
     virtual std::vector<uint8_t> Receive(const int sbuffer) override;

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
     bool SetIP(const std::string ip_address);

     /**
      * \brief Decode an hostname or ip address
      * \param ip_address The ip address or hostname
      * \return The decoded ip address
      */
     virtual std::string DecodeAddress(const std::string ip_address) override;

     /**
      * \brief Get the source ip to achieve a desired destiny
      * \param ip_address The ip address or hostname to connect
      * \return The ip address of the source
      */
     std::string GetSourceIP(const std::string ip_address) override;

     /**
      * \brief Constructor
      */
     NSocketIPv6() : NSocket("ipv6"){};
    private:
     sockaddr_in6 address_;
};

 #endif

/*
@startuml

class NSocketIPv6 {
    +address() const: sockaddr_in6
    +set_address(sockaddr_in6): void
    +CreateSocket(): bool
    +Send(const std::vector<uint8_t> &, const std::string) const: int
    +Receive(const int) const: std::vector<uint8_t>
    +Close(): bool
    +SetIP(const std::string): bool
    +DecodeAddress(const std::string): std::string
    +GetSourceIP(const std::string): std::string
    +NSocketIPv6()

    -address_: sockaddr_in6 
}

@enduml
*/