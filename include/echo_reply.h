/**
 * \file
 * \brief Echo reply class definition
 */

 #include "echo.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_REPLY_H_
#define PING_PROJECT_INCLUDE_ECHO_REPLY_H_

/**
 * \brief Echo reply class
 */
class EchoReply : public Echo
{
    public:
     /**
      * \brief Decode a message and storage parameters
      * \return The true if message is decoded and false otherwise
      */
     bool Decode(const std::vector<uint8_t> &raw_buffer) override;

     /**
      * \brief Constructor
      */
     EchoReply() : Echo(0, 0) {}; 
};

#endif

/*
@startuml

class EchoReply {
    +Decode(const vector<uint8_t> &): bool
    +EchoReply()
}

EchoReply --|> Echo

@enduml
*/