#include <stdint.h>

#include <vector>
#include <tuple>

#ifndef PING_PROJECT_INCLUDE_ICMP_H_
#define PING_PROJECT_INCLUDE_ICMP_H_

class icmp
{
    public:
     std::vector<uint8_t> Encode();
     bool Decode(std::vector<uint8_t> buffer);

     inline uint8_t Type() const {
        return type_;
     }

     inline void set_Type(uint8_t type){
        type_ = type;
     }

     inline uint8_t Code() const {
        return code_;
     }

     inline void set_Code(uint8_t code){
        code_ = code;
     }

     inline uint8_t Pointer() const {
        return pointer_;
     }

     inline void set_Pointer(uint8_t pointer){
        pointer_ = pointer;
     }

     inline uint32_t GatewayAddress() const {
        return gateway_address_;
     }

     inline void set_GatewayAddress(uint32_t gateway_address){
        gateway_address_ = gateway_address;
     }

     inline uint16_t Identifier() const {
        return identifier_;
     }

     inline void set_Identifier(uint16_t identifier){
        identifier_ = identifier;
     }

     inline uint16_t SequenceNumber() const {
        return sequence_number_;
     }

     inline void set_SequenceNumber(uint16_t sequence_number){
        sequence_number_ = sequence_number;
     }

     inline uint32_t OriginateTimeStamp() const {
        return originate_timestamp_;
     }

     inline void set_OriginateTimeStamp(uint32_t originate_timestamp){
        originate_timestamp_ = originate_timestamp;
     }

     inline uint32_t ReceiveTimeStamp() const {
        return receive_timestamp_;
     }

     inline void set_ReceiveTimeStamp(uint32_t receive_timestamp){
        receive_timestamp_ = receive_timestamp;
     }

     inline uint32_t TransmitTimeStamp() const {
        return transmit_timestamp_;
     }

     inline void set_TransmitTimeStamp(uint32_t transmit_timestamp){
        transmit_timestamp_ = transmit_timestamp;
     }

     inline std::vector<uint8_t> Data(){
        return data_;
     }

     inline void set_Data(std::vector<uint8_t> data){
        data_.reserve(data.size());
        std::copy(data.begin(), data.end(), std::back_inserter(data_));
     }

     inline void clearCheckSum() {
        checksum_ = 0;
     }

     inline void clearData() {
        data_.clear();
     }

     inline bool operator==(const icmp &other) const
     {
        return std::tie(type_, code_, data_, pointer_, gateway_address_, identifier_, sequence_number_, originate_timestamp_, receive_timestamp_, transmit_timestamp_) ==
            std::tie(other.type_, other.code_, other.data_, other.pointer_, other.gateway_address_, other.identifier_, other.sequence_number_, other.originate_timestamp_, other.receive_timestamp_, other.transmit_timestamp_);
     }
    private:
     // Global
     uint8_t type_ = UINT8_MAX;
     uint8_t code_ = UINT8_MAX;
     uint16_t checksum_ = 0;
     std::vector<uint8_t> data_ = {};

     // For type 12 (Parameter Problem Message)
     uint8_t pointer_ = 0;

     // For type 5 (Redirect Message)
     uint32_t gateway_address_ = 0;

     // For type 0, 8, 13, 14, 15 and 16 (Echo, Timestamp, Information) request and reply
     uint16_t identifier_ = 0;
     uint16_t sequence_number_ = 0;

     // For type 13 and 14 (Timestamp)
     uint32_t originate_timestamp_ = 0;
     uint32_t receive_timestamp_ = 0;
     uint32_t transmit_timestamp_ = 0;

     inline bool isDestinationUnreach() const {
        return Type() == 3 && Code() >= 0 &&  Code() <= 5;
     }

     inline bool isTimeExceed() const {
        return Type() == 11 && Code() >= 0 &&  Code() <= 1;
     }

     inline bool isParameterProblem() const {
        return Type() == 12 && Code() == 0;
     }

     inline bool isSourceQuench() const {
        return Type() == 4 && Code() == 0;
     }

     inline bool isRedirectMessage() const {
        return Type() == 5 && Code() >= 0 &&  Code() <= 3;
     }

     inline bool isEchoRequest() const {
        return Type() == 8 && Code() == 0;
     }

     inline bool isEchoReply() const {
        return Type() == 0 && Code() == 0;
     }

     inline bool isTimeStamp() const {
        return Type() == 13 && Code() == 0;
     }

     inline bool isTimeStampReply() const {
        return Type() == 14 && Code() == 0;
     }

     inline bool isInfoRequest() const {
        return Type() == 15 && Code() == 0;
     }

     inline bool isInfoReply() const {
        return Type() == 16 && Code() == 0;
     }

     uint16_t createCheckSum(std::vector<uint8_t> buffer);
     
     bool verifyCheckSum(std::vector<uint8_t> buffer);
};

#endif