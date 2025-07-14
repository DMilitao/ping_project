/**
 * \file
 * \brief ICMP class methods
 */

#include "include/icmp.h"

#include <stdint.h>

#include <iostream>
#include <vector>

bool icmp::Decode(const std::vector<uint8_t> &buffer){
    std::size_t sbuffer = buffer.size();

    if (sbuffer < 8 ) {
      return false;
    }

    if ( !verifyCheckSum(buffer) ){
      return false;
    }
     set_Type(buffer[0]);
     set_Code(buffer[1]);

     if ( isEchoRequest() || isEchoReply() || isInfoRequest() || isInfoReply() ){

        uint16_t identifier = (buffer.at(4) << 8) + buffer.at(5);
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer.at(6) << 8)  + buffer.at(7);
        set_SequenceNumber(sequence_number);

     } else if ( isEchoReply() ) {
        uint16_t identifier = (buffer.at(4) << 8) + buffer.at(5);
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer.at(6) << 8)  + buffer.at(7);
        set_SequenceNumber(sequence_number);
   
     } else if ( isTimeStamp() || isTimeStampReply() ) {
        uint16_t identifier = (buffer.at(4) << 8) + buffer.at(5);
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer.at(6) << 8)  + buffer.at(7);
        set_SequenceNumber(sequence_number);

        uint32_t originate_timestamp = (buffer.at(8) << 24) + (buffer.at(9) << 16) + (buffer.at(10) << 8) + buffer.at(11);
        set_OriginateTimeStamp(originate_timestamp);

        uint32_t receive_timestamp = (buffer.at(12) << 24) + (buffer.at(13) << 16) + (buffer.at(14) << 8) + buffer.at(15);
        set_ReceiveTimeStamp(receive_timestamp);

        uint32_t transmit_timestamp = (buffer.at(16) << 24)+ (buffer.at(17) << 16) + (buffer.at(18) << 8) + buffer.at(19);
        set_TransmitTimeStamp(transmit_timestamp);

        return true;

     } else if ( isDestinationUnreach() || isSourceQuench() || isTimeExceed() ) {

     } else if ( isRedirectMessage() ) {
        uint32_t gateway_address = (buffer.at(4) << 24)+ (buffer.at(5) << 16) + (buffer.at(6) << 8)  + buffer.at(7);
        set_GatewayAddress(gateway_address);

     } else if ( isParameterProblem() ) {
        set_Pointer(buffer.at(4));
     } else {
        return false;
     }

     std::vector<uint8_t> data(buffer.begin()+8, buffer.end());
     set_Data(data);
     return true;
}

std::vector<uint8_t> icmp::Encode(){
    std::vector<uint8_t> msg = {type_, code_, 0 , 0};

    if ( isEchoRequest() || isEchoReply() || isInfoRequest() || isInfoReply() ){
        uint8_t identifier_high = (identifier_ >> 8) & 0xFF;
        uint8_t identifier_low = (identifier_) & 0xFF;
        msg.push_back(identifier_high);
        msg.push_back(identifier_low);

        uint8_t sequence_number_high = (sequence_number_ >> 8) & 0xFF;
        uint8_t sequence_number_low = (sequence_number_) & 0xFF;
        msg.push_back(sequence_number_high);
        msg.push_back(sequence_number_low);

     } else if ( isTimeStamp() ||  isTimeStampReply() ) {
        uint8_t identifier_high = (identifier_ >> 8) & 0xFF;
        uint8_t identifier_low = (identifier_) & 0xFF;
        msg.push_back(identifier_high);
        msg.push_back(identifier_low);

        uint8_t sequence_number_high = (sequence_number_ >> 8) & 0xFF;
        uint8_t sequence_number_low = (sequence_number_) & 0xFF;
        msg.push_back(sequence_number_high);
        msg.push_back(sequence_number_low);

        uint8_t originate_timestamp_first = (originate_timestamp_ >> 24) & 0xFF;
        uint8_t originate_timestamp_second = (originate_timestamp_ >> 16) & 0xFF;
        uint8_t originate_timestamp_third = (originate_timestamp_ >> 8) & 0xFF;
        uint8_t originate_timestamp_last = (originate_timestamp_) & 0xFF;
        msg.push_back(originate_timestamp_first);
        msg.push_back(originate_timestamp_second);
        msg.push_back(originate_timestamp_third);
        msg.push_back(originate_timestamp_last);

        uint8_t receive_timestamp_first = (receive_timestamp_ >> 24) & 0xFF;
        uint8_t receive_timestamp_second = (receive_timestamp_ >> 16) & 0xFF;
        uint8_t receive_timestamp_third = (receive_timestamp_ >> 8) & 0xFF;
        uint8_t receive_timestamp_last = (receive_timestamp_) & 0xFF;
        msg.push_back(receive_timestamp_first);
        msg.push_back(receive_timestamp_second);
        msg.push_back(receive_timestamp_third);
        msg.push_back(receive_timestamp_last);

        uint8_t transmit_timestamp_first = (transmit_timestamp_ >> 24) & 0xFF;
        uint8_t transmit_timestamp_second = (transmit_timestamp_ >> 16) & 0xFF;
        uint8_t transmit_timestamp_third = (transmit_timestamp_ >> 8) & 0xFF;
        uint8_t transmit_timestamp_last = (transmit_timestamp_) & 0xFF;
        msg.push_back(transmit_timestamp_first);
        msg.push_back(transmit_timestamp_second);
        msg.push_back(transmit_timestamp_third);
        msg.push_back(transmit_timestamp_last);

     } else if ( isDestinationUnreach() || isSourceQuench() || isTimeExceed() ) {

     } else if ( isRedirectMessage() ) {
        uint8_t g_address_first = (gateway_address_ >> 24) & 0xFF;
        uint8_t g_address_second = (gateway_address_ >> 16) & 0xFF;
        uint8_t g_address_third = (gateway_address_ >> 8) & 0xFF;
        uint8_t g_address_last = (gateway_address_) & 0xFF;
        msg.push_back(g_address_first);
        msg.push_back(g_address_second);
        msg.push_back(g_address_third);
        msg.push_back(g_address_last);

     } else if ( isParameterProblem() ) {
        msg.push_back(pointer_);

     } else {
        msg.clear();
        return msg;
     }

      if ( msg.size() < 8 ) {
         msg.resize(8);
      }

      msg.insert(msg.end(),data_.begin(),data_.end());
      clearCheckSum();

      uint16_t checksum = createCheckSum(msg);
      msg.at(2) = (checksum >> 8) & 0xFF;
      msg.at(3) = (checksum) & 0xFF;

      return msg;
}

uint16_t icmp::createCheckSum(const std::vector<uint8_t> &buffer) const {
      std::size_t sbuffer = buffer.size();

      uint32_t checksum = 0;

      for (std::size_t i = 0; i < sbuffer; i += 2) {
         uint16_t op_1 = (buffer.at(i) << 8);
         uint16_t op_2 = i + 1 < sbuffer ? buffer.at(i+1) : 0;

         checksum +=  op_1 + op_2;

         while ( checksum > 0xFFFF ) {
            checksum = ( checksum & 0xFFFF ) + ( ( checksum >> 16 ) & 0xFFFF );
         } 
      }
      return ~checksum & 0xFFFF;
};

bool icmp::verifyCheckSum(const std::vector<uint8_t> &buffer) const {
      return (int)createCheckSum(buffer) == 0 ? true : false;
}