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
class icmp
{
    public:
     /**
      * \brief Encode the parameter into a message according by the type and code
      * \return The message
      */
     std::vector<uint8_t> Encode();

     /**
      * \brief Decode a message and storage parameters
      * \return The true if message is decoded and false otherwise
      */
     bool Decode(const std::vector<uint8_t> &buffer);

     /**
      * \brief Get message type
      * \return The message type
      */
     inline uint8_t Type() const {
        return type_;
     }

     /**
      * \brief Set message type
      * \param type The desired message type
      */
     inline void set_Type(uint8_t type){
        type_ = type;
     }

     /**
      * \brief Get message code
      * \return The message code
      */
     inline uint8_t Code() const {
        return code_;
     }

     /**
      * \brief Set message code
      * \param code The desired message code
      */
     inline void set_Code(uint8_t code){
        code_ = code;
     }

     /**
      * \brief Get message pointer in Parameter Problem messages
      * \return The message pointer
      */
     inline uint8_t Pointer() const {
        return pointer_;
     }

     /**
      * \brief Set pointer in Parameter Problem messages
      * \param pointer The desired pointer
      */
     inline void set_Pointer(uint8_t pointer){
        pointer_ = pointer;
     }

     /**
      * \brief Get message gateway address in Redirect messages
      * \return The message gateway address
      */
     inline uint32_t GatewayAddress() const {
        return gateway_address_;
     }

     /**
      * \brief Set message gateway address in Redirect messages
      * \param gateway_address The desired message gateway address
      */
     inline void set_GatewayAddress(uint32_t gateway_address){
        gateway_address_ = gateway_address;
     }

     /**
      * \brief Get message identifier
      * \return The message identifier
      */
     inline uint16_t Identifier() const {
        return identifier_;
     }

     /**
      * \brief Set message identifier
      * \param identifier The desired message pointer
      */
     inline void set_Identifier(uint16_t identifier){
        identifier_ = identifier;
     }

     /**
      * \brief Get message sequence number
      * \return The message sequence number
      */
     inline uint16_t SequenceNumber() const {
        return sequence_number_;
     }

     /**
      * \brief Set message sequence number
      * \param sequence_number The desired message sequence number
      */
     inline void set_SequenceNumber(uint16_t sequence_number){
        sequence_number_ = sequence_number;
     }

      /**
      * \brief Get message originate timestamp
      * \return The message originate timestamp
      */
     inline uint32_t OriginateTimeStamp() const {
        return originate_timestamp_;
     }

     /**
      * \brief Set message originate timestamp
      * \param originate_timestamp The desired message originate timestamp
      */
     inline void set_OriginateTimeStamp(uint32_t originate_timestamp){
        originate_timestamp_ = originate_timestamp;
     }

      /**
      * \brief Get message receive timestamp
      * \return The message receive timestamp
      */
     inline uint32_t ReceiveTimeStamp() const {
        return receive_timestamp_;
     }

     /**
      * \brief Set message receive timestamp
      * \param receive_timestamp The desired message receive timestamp
      */
     inline void set_ReceiveTimeStamp(uint32_t receive_timestamp){
        receive_timestamp_ = receive_timestamp;
     }

     /**
      * \brief Get message transmit timestamp
      * \return The message transmit timestamp
      */
     inline uint32_t TransmitTimeStamp() const {
        return transmit_timestamp_;
     }

     /**
      * \brief Set message transmit timestamp
      * \param transmit_timestamp The desired message transmit timestamp
      */
     inline void set_TransmitTimeStamp(uint32_t transmit_timestamp){
        transmit_timestamp_ = transmit_timestamp;
     }

     /**
      * \brief Get message data
      * \param data The message data
      */
     inline std::vector<uint8_t> Data(){
        return data_;
     }

     /**
      * \brief Set message data
      * \param data The desired message data
      */
     inline void set_Data(const std::vector<uint8_t> &data){
        data_.reserve(data.size());
        std::copy(data.begin(), data.end(), std::back_inserter(data_));
     }

      /**
      * \brief Clear the checksum parameter
      */
     inline void clearCheckSum() {
        checksum_ = 0;
     }

     /**
      * \brief Clear the data parameter
      */
     inline void clearData() {
        data_.clear();
     }

     /**
      * \brief Compare all fields of two icmp variables
      */
     inline bool operator==(const icmp &other) const
     {
        return std::tie(type_, code_, data_, pointer_, gateway_address_, identifier_, sequence_number_, originate_timestamp_, receive_timestamp_, transmit_timestamp_) ==
            std::tie(other.type_, other.code_, other.data_, other.pointer_, other.gateway_address_, other.identifier_, other.sequence_number_, other.originate_timestamp_, other.receive_timestamp_, other.transmit_timestamp_);
     }

     /**
      * \brief Verify if it is Destination Unreachable type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isDestinationUnreach() const {
        return Type() == 3 && Code() >= 0 &&  Code() <= 5;
     }

     /**
      * \brief Verify if it is Time Exceed type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isTimeExceed() const {
        return Type() == 11 && Code() >= 0 &&  Code() <= 1;
     }

     /**
      * \brief Verify if it is Parameter Problem type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isParameterProblem() const {
        return Type() == 12 && Code() == 0;
     }

     /**
      * \brief Verify if it is Source Quench type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isSourceQuench() const {
        return Type() == 4 && Code() == 0;
     }

     /**
      * \brief Verify if it is Redirect Message type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isRedirectMessage() const {
        return Type() == 5 && Code() >= 0 &&  Code() <= 3;
     }

     /**
      * \brief Verify if it is Echo Request type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isEchoRequest() const {
        return Type() == 8 && Code() == 0;
     }

     /**
      * \brief Verify if it is Echo Reply type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isEchoReply() const {
        return Type() == 0 && Code() == 0;
     }

     /**
      * \brief Verify if it is TimeStamp type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isTimeStamp() const {
        return Type() == 13 && Code() == 0;
     }

     /**
      * \brief Verify if it is TimeStamp Reply type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isTimeStampReply() const {
        return Type() == 14 && Code() == 0;
     }

     /**
      * \brief Verify if it is Information Request type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isInfoRequest() const {
        return Type() == 15 && Code() == 0;
     }

     /**
      * \brief Verify if it is Information Reply type of message
      * \return true if type and code correpond and false otherwise
      */
     inline bool isInfoReply() const {
        return Type() == 16 && Code() == 0;
     }

    private:
     uint8_t type_ = UINT8_MAX;        //!< Type of message not initialized in zero
     uint8_t code_ = UINT8_MAX;        //!< Code of message not initialized in zero
     uint16_t checksum_ = 0;           //!< Checksum of the message
     std::vector<uint8_t> data_ = {};  //!< Data of the message

     uint8_t pointer_ = 0;             //!< Pointer for Parameter problem messages

     uint32_t gateway_address_ = 0;    //!< Gateway Address for Redirect messages

     uint16_t identifier_ = 0;         //!< Identifier of message for Echo, Timestamp and Info messages
     uint16_t sequence_number_ = 0;    //!< Sequence Number of message for Echo, Timestamp and Info messages

     uint32_t originate_timestamp_ = 0;//!< Originate Timestamp of message for Timestamp messages
     uint32_t receive_timestamp_ = 0;  //!< Originate Timestamp of message for Timestamp messages
     uint32_t transmit_timestamp_ = 0; //!< Originate Timestamp of message for Timestamp messages

     /**
      * \brief Create the 16-bit verification word of a message
      * \param buffer The message
      * \return The 16-bit verification word
      */
     uint16_t createCheckSum(const std::vector<uint8_t> &buffer) const;

     /**
      * \brief Verify that the receveid message is complete by checking the 16-bit verification word
      * \param buffer The received message
      * \return true if the message is complete and false otherwise
      */
     bool verifyCheckSum(const std::vector<uint8_t> &buffer) const;
};

#endif

/*
@startuml

class icmp {
    +Encode(): vector<uint8_t>
    +Decode(const vector<uint8_t> &): bool
    +Type() const: uint8_t
    +set_Type(uint8_t): void
    +Code() const: uint8_t
    +set_Code(uint8_t): void
    +Pointer() const: uint8_t
    +set_Pointer(uint8_t): void
    +GatewayAddress() const: uint32_t
    +set_GatewayAddress(uint32_t): void
    +Identifier() const: uint16_t
    +set_Identifier(uint16_t): void
    +SequenceNumber() const: uint16_t
    +set_SequenceNumber(uint16_t): void
    +OriginateTimeStamp() const: uint32_t
    +set_OriginateTimeStamp(uint32_t): void
    +ReceiveTimeStamp() const: uint32_t
    +set_ReceiveTimeStamp(uint32_t): void
    +TransmitTimeStamp() const: uint32_t
    +set_TransmitTimeStamp(uint32_t): void
    +Data(): vector<uint8_t>
    +set_Data(const vector<uint8_t> &): void
    +clearCheckSum(): void
    +clearData():void
    +operator==(const icmp &) const: bool
    +isDestinationUnreach() const: bool
    +isTimeExceed() const: bool
    +isParameterProblem() const: bool
    +isSourceQuench() const: bool
    +isRedirectMessage() const: bool
    +isEchoRequest() const: bool
    +isEchoReply() const: bool
    +isTimeStamp() const: bool
    +isTimeStampReply(): const bool
    +isInfoRequest() const: bool
    +isinfoReply() const: bool


    -type_: uint8_t       
    -code_: uint8_t  
    -checksum_: uint16_t
    -data_: vector<uint8_t>  
    -pointer_: uint8_t  
    -gateway_address_: uint32_t 
    -identifier_: uint16_t 
    -sequence_number_: uint16_t 
    -originate_timestamp_: uint32_t
    -receive_timestamp_: uint32_t
    -transmit_timestamp_: uint32_t 
    -createCheckSum(const vector<uint8_t> &) const: uint16_t
    -verifyCheckSum(const vector<uint8_t> &) const: bool
}

@enduml
*/