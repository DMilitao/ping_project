/**
 * \file
 * \brief Echo request class definition
 */

#include "echo.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_REQUEST_H_
#define PING_PROJECT_INCLUDE_ECHO_REQUEST_H_

/**
 * \brief Echo request class
 */
class EchoRequest : public Echo 
{
   public:
     /**
      * \brief Decode a message and storage parameters
      * \param raw_buffer The received message
      * \return The true if message is decoded and false otherwise
      */
     virtual bool Decode(const std::vector<uint8_t> &raw_buffer) override;

     /**
      * \brief Constructor
      */
     EchoRequest() : Echo(8, 0) {}; 
};

#endif

/*
@startuml

class EchoRequest {
    +Decode(const vector<uint8_t> &): bool
    +EchoRequest()
}

EchoRequest --|> Echo

@enduml
*/