/**
 * \file
 * \brief Echo reply class methods
 */
#include "include/echo_reply.h"

#include "include/icmp.h"

bool EchoReply::Decode(const std::vector<uint8_t> &raw_buffer){
   std::vector<uint8_t> buffer(raw_buffer.begin()+(4*(raw_buffer.at(0) & 0x0F)),raw_buffer.end());

   return Echo::Decode(buffer);
}