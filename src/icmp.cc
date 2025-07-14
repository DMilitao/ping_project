#include "include/icmp.h"

#include <stdint.h>

#include <iostream>
#include <vector>

bool icmp::Decode(std::vector<uint8_t> buffer){
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

        uint16_t identifier = (buffer[4] << 8) + buffer[5];
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer[6] << 8)  + buffer[7];
        set_SequenceNumber(sequence_number);

     } else if ( isEchoReply() ) {

        uint16_t identifier = (buffer[4] << 8) + buffer[5];
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer[6] << 8)  + buffer[7];
        set_SequenceNumber(sequence_number);
        
     } else if ( isTimeStamp() || isTimeStampReply() ) {

        uint16_t identifier = (buffer[4] << 8) + buffer[5];
        set_Identifier(identifier);

        uint16_t sequence_number = (buffer[6] << 8)  + buffer[7];
        set_SequenceNumber(sequence_number);

        uint32_t originate_timestamp = (buffer[8] << 24) + (buffer[9] << 16) + (buffer[10] << 8) + buffer[11];
        set_OriginateTimeStamp(originate_timestamp);

        uint32_t receive_timestamp = (buffer[12] << 24) + (buffer[13] << 16) + (buffer[14] << 8) + buffer[15];
        set_ReceiveTimeStamp(receive_timestamp);

        uint32_t transmit_timestamp = (buffer[16] << 24)+ (buffer[17] << 16) + (buffer[18] << 8) + buffer[19];
        set_TransmitTimeStamp(transmit_timestamp);

        return true;
     } else if ( isDestinationUnreach() || isSourceQuench() || isTimeExceed() ) {

     } else if ( isRedirectMessage() ) {
        uint32_t gateway_address = (buffer[4] << 24)+ (buffer[5] << 16) + (buffer[6] << 8)  + buffer[7];
        set_GatewayAddress(gateway_address);

     } else if ( isParameterProblem() ) {
        set_Pointer(buffer[4]);
     } else {
      
        return false;
     }
     
     std::vector<uint8_t> data(buffer.begin()+8, buffer.end());
     set_Data(data);
     return true;
}

std::vector<uint8_t> icmp::Encode(){
    std::vector<uint8_t> msg;
    
    msg.push_back(type_);
    msg.push_back(code_);
    msg.push_back(0);
    msg.push_back(0);
    
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

      while ( msg.size() < 8 ) {
         msg.push_back(0);
      }

      msg.insert(msg.end(),data_.begin(),data_.end());
      clearCheckSum();

      uint16_t checksum = createCheckSum(msg);
      msg[2] = (checksum >> 8) & 0xFF;
      msg[3] = (checksum) & 0xFF;

      return msg;
}

uint16_t icmp::createCheckSum(std::vector<uint8_t> buffer){
      std::size_t sbuffer = buffer.size();
      if ( sbuffer % 2) {
         buffer.push_back(0);
      }

      uint32_t checksum = 0;

      for (std::size_t i = 0; i < buffer.size(); i += 2) {
         checksum += (buffer[i] << 8) + buffer[i+1];
         
         while ( checksum > 0xFFFF ) {
            checksum = ( checksum & 0xFFFF ) + ( ( checksum >> 16 ) & 0xFFFF );
         } 
      }
      return ~checksum & 0xFFFF;
};

bool icmp::verifyCheckSum(std::vector<uint8_t> buffer){
      return (int)createCheckSum(buffer) == 0 ? true : false;
}