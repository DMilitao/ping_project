/**
 * \file
 * \brief ICMP class methods
 */

#include "include/icmp.h"

#include <stdint.h>
#include <iostream>
#include <vector>

bool Icmp::Decode(const std::vector<uint8_t> &buffer){
   return false;
}

std::vector<uint8_t> Icmp::Encode(){
   std::vector<uint8_t> msg = {};
   return msg;
}

uint16_t Icmp::createCheckSum(const std::vector<uint8_t> &buffer){
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
   checksum_ = ~checksum & 0xFFFF;
   return checksum_;
};

bool Icmp::verifyCheckSum(const std::vector<uint8_t> &buffer){
   return (int)createCheckSum(buffer) == 0 ? true : false;
}

void Icmp::clear_fields() {
    type_ = UINT8_MAX;
    code_ = UINT8_MAX;
    checksum_ = 0;
    data_.clear();
}