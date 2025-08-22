#include <gmock/gmock.h>

#include "include/nsocket_ipv6.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSOCKET_IPV6_H_
#define PING_PROJECT_INCLUDE_MOCK_NSOCKET_IPV6_H_

class MockNSocketIPv6 : public NSocketIPv6 {
   public:
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data, const std::string ip_address), (const, override));
    MOCK_METHOD(std::vector<uint8_t>, Receive, (const int sbuffer), (override));
    MOCK_METHOD(std::string, last_ip_com, (), (const, override));
};

#endif 
