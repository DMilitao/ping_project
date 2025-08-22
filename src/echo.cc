/**
 * \file
 * \brief Echo class methods
 */
#include "include/echo.h"

#include "include/icmp.h"

bool Echo::Decode(const std::vector<uint8_t> &raw_buffer){
   std::size_t sbuffer = raw_buffer.size();

   set_type(raw_buffer[0]);
   set_code(raw_buffer[1]);
   verifyCheckSum(raw_buffer);

   if ( sbuffer < 8 || !isEcho() ){
    clear_fields();
    return false;
   }

   uint16_t identifier = (raw_buffer.at(4) << 8) + raw_buffer.at(5);
   set_identifier(identifier);

   uint16_t sequence_number = (raw_buffer.at(6) << 8)  + raw_buffer.at(7);
   set_sequence_number(sequence_number);

   std::vector<uint8_t> data(raw_buffer.begin()+8, raw_buffer.end());
   set_data(data);
  
   return true;
}

std::vector<uint8_t> Echo::Encode(){
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

int Echo::isEqual(const Echo &other) const {
   if ( std::make_tuple(type(), code(), identifier(), sequence_number(), data()) == std::make_tuple(other.type(), other.code(),other.identifier(),other.sequence_number(), other.data()) ) {
      return 1;
   } else if (std::make_tuple(identifier(), sequence_number(), data()) == std::make_tuple(other.identifier(),other.sequence_number(), other.data()) ) {
      return 2;
   } else {
      return 0;
   }
}

void Echo::clear_fields() {
    Icmp::clear_fields();
    identifier_ = 0;
    sequence_number_ = 0;
}