/**
 * \file
 * \brief Echo IPv6 class methods
 */
#include "include/echo_ipv6.h"

#include <arpa/inet.h>

std::vector<uint8_t> EchoIPv6::Encode(){

   std::vector<uint8_t> msg = Echo::Encode();
   clear_checksum();
   msg.at(2) = 0;
   msg.at(3) = 0;

   std::vector<uint8_t> msg_for_checksum = {};
   struct in6_addr src_addr;
   inet_pton(AF_INET6, ip_source().c_str(), &src_addr);
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&src_addr), reinterpret_cast<uint8_t*>(&src_addr) + sizeof(struct in6_addr));
   
   struct in6_addr dst_addr;
   inet_pton(AF_INET6, ip_destiny().c_str(), &dst_addr);
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&dst_addr), reinterpret_cast<uint8_t*>(&dst_addr) + sizeof(struct in6_addr));

   uint32_t icmp_len = htonl(msg.size());
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&icmp_len), reinterpret_cast<uint8_t*>(&icmp_len) + sizeof(uint32_t));

   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(58);

   msg_for_checksum.insert(msg_for_checksum.end(),msg.begin(),msg.end());

   uint16_t checksum = createCheckSum(msg_for_checksum);
   msg.at(2) = (checksum >> 8) & 0xFF;
   msg.at(3) = (checksum) & 0xFF;

   return msg;
}

bool EchoIPv6::Decode(const std::vector<uint8_t> &raw_buffer){

   bool ans_dec = Echo::Decode(raw_buffer);

   clear_checksum();

   std::vector<uint8_t> msg_for_checksum = {};
   struct in6_addr src_addr;
   inet_pton(AF_INET6, ip_source().c_str(), &src_addr);
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&src_addr), reinterpret_cast<uint8_t*>(&src_addr) + sizeof(struct in6_addr));
   
   struct in6_addr dst_addr;
   inet_pton(AF_INET6, ip_destiny().c_str(), &dst_addr);
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&dst_addr), reinterpret_cast<uint8_t*>(&dst_addr) + sizeof(struct in6_addr));

   uint32_t icmp_len = htonl(raw_buffer.size());
   msg_for_checksum.insert(msg_for_checksum.end(), reinterpret_cast<uint8_t*>(&icmp_len), reinterpret_cast<uint8_t*>(&icmp_len) + sizeof(uint32_t));

   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(0);
   msg_for_checksum.push_back(58);

   msg_for_checksum.insert(msg_for_checksum.end(),raw_buffer.begin(),raw_buffer.end());

   verifyCheckSum(msg_for_checksum);

   return ans_dec;
}