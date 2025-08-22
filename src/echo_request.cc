/**
 * \file
 * \brief Echo request class methods
 */
#include "include/echo_request.h"

#include "include/icmp.h"

bool EchoRequest::Decode(const std::vector<uint8_t> &raw_buffer){
   std::vector<uint8_t> buffer(raw_buffer.begin()+(4*(raw_buffer.at(0) & 0x0F)),raw_buffer.end());

   return Echo::Decode(buffer);
}