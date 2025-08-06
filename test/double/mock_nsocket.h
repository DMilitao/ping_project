#include <gmock/gmock.h>

#include "include/echo_request.h"
#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_
#define PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_

class MockNSocket : public NSocket {
   public:
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data, const std::string ip_address), (const, override));
    MOCK_METHOD(std::vector<uint8_t>, Receive, (const int sbuffer), (const, override));
};

#endif 
