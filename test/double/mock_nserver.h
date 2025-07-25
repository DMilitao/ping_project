#include <gmock/gmock.h>

#include "include/echo_request.h"
#include "include/nserver.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSERVER_H_
#define PING_PROJECT_INCLUDE_MOCK_NSERVER_H_

class MockNServer : public NServer {
   public:
    MockNServer() : NServer() {

        EXPECT_CALL(*this, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
                return (int)data_arg.size();
            }));

        EXPECT_CALL(*this, WaitingMessage(testing::_)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
                std::vector<uint8_t> data = {1, 2, 3, 4};

                uint16_t identifier = 0xABCD;
                uint16_t sequence_number = 0xCDEF;
                EchoRequest expect_echo_request_;

                expect_echo_request_.set_data(data);
                expect_echo_request_.set_identifier(identifier);
                expect_echo_request_.set_sequence_number(sequence_number);

                std::vector<uint8_t> msg = expect_echo_request_.Encode();
                
                if (msg.size() > sbuffer){
                    msg.resize(sbuffer);
                }
                
                return msg;
            }));

        EXPECT_CALL(*this, HandleMessage(testing::A<const std::vector<uint8_t> &>())).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t> &msg) {              
                return msg.size();
            }));

    }
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data, const std::string ip_address), (const, override));
    MOCK_METHOD(std::vector<uint8_t>, WaitingMessage, (const int sbuffer), (const, override));
    MOCK_METHOD(int, HandleMessage, (const std::vector<uint8_t> &msg), (const, override));
};



#endif 
