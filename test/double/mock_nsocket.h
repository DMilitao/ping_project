#include <gmock/gmock.h>

#include "include/echo_request.h"
#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_
#define PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_

class MockNSocket : public NSocket {
   public:
    MockNSocket() : NSocket() {
        EXPECT_CALL(*this, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
                return (int)data_arg.size();
            }));
    }
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data, const std::string ip_address), (const, override));
    MOCK_METHOD(std::vector<uint8_t>, Receive, (const int sbuffer), (const, override));
};

#endif 
