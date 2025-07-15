/**
 * \file
 * \brief Echo reply class methods
 */
#include "include/echo_reply.h"

#include <stdint.h>

#include <vector>

#include "include/icmp.h"

bool EchoReply::Decode(const std::vector<uint8_t> &buffer){
   std::size_t sbuffer = buffer.size();
   
   set_type(buffer[0]);
   set_code(buffer[1]);

   if ( sbuffer < 8 || !verifyCheckSum(buffer) || !isEchoReply() ){
    clear_fields();
    return false;
   }

   uint16_t identifier = (buffer.at(4) << 8) + buffer.at(5);
   set_identifier(identifier);

   uint16_t sequence_number = (buffer.at(6) << 8)  + buffer.at(7);
   set_sequence_number(sequence_number);

   std::vector<uint8_t> data(buffer.begin()+8, buffer.end());
   set_data(data);
  
   return true;
}

std::vector<uint8_t> EchoReply::Encode(){
   set_type(type_default_);
   set_code(code_default_);

   std::vector<uint8_t> msg = {type(), code(), 0, 0};

   uint8_t identifier_high = (identifier_ >> 8) & 0xFF;
   uint8_t identifier_low = (identifier_) & 0xFF;
   msg.push_back(identifier_high);
   msg.push_back(identifier_low);

   uint8_t sequence_number_high = (sequence_number_ >> 8) & 0xFF;
   uint8_t sequence_number_low = (sequence_number_) & 0xFF;
   msg.push_back(sequence_number_high);
   msg.push_back(sequence_number_low);

   std::vector<uint8_t> data_msg(data());
   msg.insert(msg.end(),data_msg.begin(),data_msg.end());
   clear_checksum();

   uint16_t checksum = createCheckSum(msg);
   msg.at(2) = (checksum >> 8) & 0xFF;
   msg.at(3) = (checksum) & 0xFF;

   return msg;
}

void EchoReply::clear_fields() {
    Icmp::clear_fields();
    identifier_ = 0;
    sequence_number_ = 0;
}