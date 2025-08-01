/**
 * \file
 * \brief ICMP class definition
 */
#include <stdint.h>

#include <vector>
#include <tuple>

#ifndef PING_PROJECT_INCLUDE_ICMP_H_
#define PING_PROJECT_INCLUDE_ICMP_H_

/**
 * \brief ICMP class
 */
class Icmp
{
    public:
     /**
      * \brief Encode the parameter into a message according by the type and code
      * \return The message
      */
     virtual std::vector<uint8_t> Encode();

     /**
      * \brief Decode a message and storage parameters
      * \return The true if message is decoded and false otherwise
      */
     virtual bool Decode(const std::vector<uint8_t> &raw_buffer);

     /**
      * \brief Get message type
      * \return The message type
      */
     inline uint8_t type() const {
        return type_;
     }

     /**
      * \brief Set message type
      * \param type The desired message type
      */
     inline void set_type(uint8_t type){
        type_ = type;
     }

     /**
      * \brief Get message code
      * \return The message code
      */
     inline uint8_t code() const {
        return code_;
     }

     /**
      * \brief Set message code
      * \param code The desired message code
      */
     inline void set_code(uint8_t code){
        code_ = code;
     }

     /**
      * \brief Get message data
      * \param data The message data
      */
     inline std::vector<uint8_t> data() const{
        return data_;
     }

     /**
      * \brief Set message data
      * \param data The desired message data
      */
     inline void set_data(const std::vector<uint8_t> &data){
        data_.reserve(data.size());
        std::copy(data.begin(), data.end(), std::back_inserter(data_));
     }

     /**
      * \brief Set message data
      * \param data The desired message data
      */
     inline uint16_t checksum(){
        return checksum_;
     }

      /**
      * \brief Clear the checksum parameter
      */
     inline void clear_checksum() {
        checksum_ = 0;
     }

     /**
      * \brief Clear the data parameter
      */
     inline void clear_data() {
        data_.clear();
     }

     /**
      * \brief Compare all fields of two icmp variables
      */
     inline bool operator==(const Icmp &other) const
     {
        return std::make_tuple(type_, code_, data_) == std::make_tuple(other.type_, other.code_, other.data_);
     }

     /**
      * \brief Restart all fields for default value 
      */
     virtual void clear_fields();

     /**
      * \brief Create the 16-bit verification word of a message
      * \param buffer The message
      * \return The 16-bit verification word
      */
     uint16_t createCheckSum(const std::vector<uint8_t> &buffer);

     /**
      * \brief Verify that the receveid message is complete by checking the 16-bit verification word
      * \param buffer The received message
      * \return true if the message is complete and false otherwise
      */
     bool verifyCheckSum(const std::vector<uint8_t> &buffer);

    private:
     uint8_t type_ = UINT8_MAX;        //!< Type of message not initialized in zero
     uint8_t code_ = UINT8_MAX;        //!< Code of message not initialized in zero
     uint16_t checksum_ = 0;           //!< Checksum of the message
     std::vector<uint8_t> data_ = {};  //!< Data of the message
};

#endif

/*
@startuml

class Icmp {
    +Encode(): vector<uint8_t>
    +Decode(const vector<uint8_t> &): bool
    +type() const: uint8_t
    +set_type(uint8_t): void
    +code() const: uint8_t
    +set_code(uint8_t): void
    +data(): vector<uint8_t>
    +set_data(const vector<uint8_t> &): void
    +clear_checksum(): void
    +clear_data():void
    +operator==(const Icmp &) const: bool
    +clear_fields(): void
    +createCheckSum(const vector<uint8_t> &) const: uint16_t
    +verifyCheckSum(const vector<uint8_t> &) const: bool
    
    -type_: uint8_t       
    -code_: uint8_t  
    -checksum_: uint16_t
    -data_: vector<uint8_t>  
}

@enduml
*/