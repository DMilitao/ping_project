/**
 * \file
 * \brief Echo IPv6 class definition
 */
#include "echo.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_IPV6_H_
#define PING_PROJECT_INCLUDE_ECHO_IPV6_H_

/**
 * \brief Echo IPv6 class
 */
class EchoIPv6 : public Echo
{
    public:
     /**
      * \brief Encode the parameter into a message according by the type and code
      * \return The message encoded
      */
     std::vector<uint8_t> Encode() override;

     /**
      * \brief Decode a message and storage parameters
      * \param raw_buffer The received message
      * \return The true if message is decoded and false otherwise
      */
     virtual bool Decode(const std::vector<uint8_t> &raw_buffer) override;

     /**
      * \brief Constructor
      * \param type The type of Echo
      * \param code The code of Echo
      */
     EchoIPv6(uint8_t type, uint8_t code) : Echo(type, code) {};
};

#endif

/*
@startuml

class EchoIPv6 {
    +Encode(): vector<uint8_t>
    +Decode(const vector<uint8_t> &): bool
    +EchoIPv6(uint8_t, uint8_t)
}

EchoIPv6 --|> Echo

@enduml
*/