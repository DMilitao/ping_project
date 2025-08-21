/**
 * \file
 * \brief Echo class definition
 */
#include <string>

#include "icmp.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_H_
#define PING_PROJECT_INCLUDE_ECHO_H_

/**
 * \brief Echo class
 */
class Echo: public Icmp 
{
    public:
     /**
      * \brief Encode the parameter into a message according by the type and code
      * \return The message
      */
     virtual std::vector<uint8_t> Encode() override;

     /**
      * \brief Decode a message and storage parameters
      * \return The true if message is decoded and false otherwise
      */
     virtual bool Decode(const std::vector<uint8_t> &raw_buffer) override;

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
      * \brief Get ip source for message
      * \return The ip source
      */
     inline std::string ip_source() const {
        return ip_source_;
     }

     /**
      * \brief Set message ip source
      * \param ip_source The desired message ip source
      */
     inline void set_ip_source(std::string ip_source){
        ip_source_ = ip_source;
     }

     /**
      * \brief Get ip destiny for message
      * \return The ip destiny
      */
     inline std::string ip_destiny() const {
        return ip_destiny_;
     }

     /**
      * \brief Set message ip destiny
      * \param ip_destiny The desired message ip destiny
      */
     inline void set_ip_destiny(std::string ip_destiny){
        ip_destiny_ = ip_destiny;
     }

     /**
      * \brief Compare fields of two Echo objects
      * \return 1 if all parameters are 2 if only sequence number, identifier and data are equal and 0 otherwise
      */
     int isEqual(const Echo &other) const;

     /**
      * \brief Verify if it is Echo type of message
      * \return true if type and code correspond and false otherwise
      */
     inline bool isEcho() const {
        return type() == type_default_ && code() == code_default_;
     }

     /**
      * \brief Restart all fields for default value
      */
     void clear_fields() override;

     /**
      * \brief Get default type of the message object
      * \return The default type
      */
     uint8_t type_default(){
      return type_default_;
     }

     /**
      * \brief Get default code of the message object
      * \return The default code
      */
     uint8_t code_default(){
      return code_default_;
     }

     /**
      * \brief Constructor
      * \param type The type of Echo
      * \param code The code of Echo
      */
     Echo(uint8_t type, uint8_t code) : type_default_(type), code_default_(code) {}; 
    
     private:
     uint8_t type_default_ = 8;  //!< Default type of message
     uint8_t code_default_ = 0;  //!< Default code of message
     uint16_t identifier_ = 0;         //!< Identifier of message
     uint16_t sequence_number_ = 0;    //!< Sequence Number of message
     std::string ip_source_ = "";
     std::string ip_destiny_ = "";
};

#endif

/*
@startuml

class Echo {
    +Encode(): vector<uint8_t>
    +Decode(const vector<uint8_t> &): bool
    +identifier() const: uint16_t
    +set_identifier(uint16_t): void
    +sequence_number() const: uint16_t
    +set_sequence_number(uint16_t): void
    +ip_source() const: std::string
    +set_ip_source(std::string): void
    +ip_destiny() const: std::string
    +set_ip_destiny(std::string): void
    +isEqual(const Echo &) const: bool
    +IsEcho(): bool
    +clear_fields(): void
    +type_default(): uint8_t
    +code_default(): uint8_t
    +Echo(uint8_t type, uint8_t)


    -type_default_: uint8_t       
    -code_default_: uint8_t  
    -identifier_: uint16_t
    -sequence_number_: uint16_t
    -ip_source_: std::string
    -ip_destiny_: std::string
}

Echo --|> Icmp

@enduml
*/