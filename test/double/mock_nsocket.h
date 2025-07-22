#include <gmock/gmock.h>

#include "include/echo_request.h"

#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_
#define PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_

class MockNSocket : public NSocket {
   public:
    MockNSocket() : NSocket() {

        EXPECT_CALL(*this, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, uint32_t ip) {
                return (int)data_arg.size();
            }));

        EXPECT_CALL(*this, Receive(testing::_)).WillRepeatedly(testing::Invoke([](int sbuffer) {
                uint8_t type = 8;
                uint8_t code = 0;
                std::vector<uint8_t> data = {1, 2, 3, 4};

                uint16_t identifier = 0xABCD;
                uint16_t sequence_number = 0xCDEF;
                EchoRequest expect_echo_request_;

                expect_echo_request_.set_type(type);
                expect_echo_request_.set_code(code);
                expect_echo_request_.set_data(data);
                expect_echo_request_.set_identifier(identifier);
                expect_echo_request_.set_sequence_number(sequence_number);

                std::vector<uint8_t> msg = expect_echo_request_.Encode();
                
                if (msg.size() > sbuffer){
                    msg.resize(sbuffer);
                }
                
                return msg;
            }));
    }
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data, uint32_t ip_address), (override));
    MOCK_METHOD(std::vector<uint8_t>, Receive, (int sbuffer), (override));
};



#endif 
