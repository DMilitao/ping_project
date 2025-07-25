/**
 * \file
 * \brief Echo request class definition
 */
#include <stdint.h>

#include <vector>
#include <tuple>

#include "echo_reply.h"
#include "icmp.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_REQUEST_H_
#define PING_PROJECT_INCLUDE_ECHO_REQUEST_H_

/**
 * \brief Echo request class
 */
class EchoRequest : public Icmp 
{
    public:
     /**
      * \brief Encode the parameter into a message according by the type and code
      * \return The message
      */
     std::vector<uint8_t> Encode() override;

     /**
      * \brief Decode a message and storage parameters
      * \return The true if message is decoded and false otherwise
      */
     bool Decode(const std::vector<uint8_t> &buffer) override;

     /**
      * \brief Get message identifier
      * \return The message identifier
      */
     inline uint16_t identifier() const {
        return identifier_;
     }

     /**
      * \brief Set message identifier
      * \param identifier The desired message pointer
      */
     inline void set_identifier(uint16_t identifier){
        identifier_ = identifier;
     }

     /**
      * \brief Get message sequence number
      * \return The message sequence number
      */
     inline uint16_t sequence_number() const {
        return sequence_number_;
     }

     /**
      * \brief Set message sequence number
      * \param sequence_number The desired message sequence number
      */
     inline void set_sequence_number(uint16_t sequence_number){
        sequence_number_ = sequence_number;
     }

     /**
      * \brief Compare all fields of two icmp variables
      * \return true if parameters are equal and false otherwise
      */
     inline bool operator==(const EchoRequest &other) const
     {
        return std::make_tuple(type(), code(), identifier(), sequence_number(), data()) == std::make_tuple(other.type(), other.code(),other.identifier(),other.sequence_number(), other.data());
     }

     /**
      * \brief Compare identifier, sequence number and data of an Echo Reply message
      * \return true if parameters are equal and false otherwise
      */
     inline bool operator==(const EchoReply &other) const
     {
        return std::make_tuple(identifier(), sequence_number(), data()) == std::make_tuple(other.identifier(),other.sequence_number(), other.data());
     }

     /**
      * \brief Verify if it is Echo Request type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isEchoRequest() const {
        return type() == type_default_ && code() == code_default_;
     }

     /**
      * \brief Restart all fields for default value
      */
     void clear_fields() override;

    private:
     static const uint8_t type_default_ = 8;  //!< Default type of message
     static const uint8_t code_default_ = 0;  //!< Default code of message
     uint16_t identifier_ = 0;         //!< Identifier of message
     uint16_t sequence_number_ = 0;    //!< Sequence Number of message
};

#endif

/*
@startuml

class EchoRequest {
    +Encode(): vector<uint8_t>
    +Decode(const vector<uint8_t> &): bool
    +identifier() const: uint16_t
    +set_identifier(uint16_t): void
    +sequence_number() const: uint16_t
    +set_sequence_number(uint16_t): void
    +operator==(const EchoRequest &) const: bool
    +operator==(const EchoReply &) const: bool
    +IsEchoRequest(): bool
    +clear_fields(): void

    -type_default_: uint8_t       
    -code_default_: uint8_t  
    -identifier_: uint16_t
    -sequence_number_: uint16_t
}

EchoRequest --|> Icmp

@enduml
*/