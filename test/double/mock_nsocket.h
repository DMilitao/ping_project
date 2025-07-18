#include <gmock/gmock.h>

#include "include/echo_request.h"

#include "include/nsocket.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_
#define PING_PROJECT_INCLUDE_MOCK_NSOCKET_H_

class MockNSocket : public NSocket {
   public:

    MockNSocket() : NSocket() {

        EXPECT_CALL(*this, Send(testing::A<const std::vector<uint8_t> &>())).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg) {
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

        EXPECT_CALL(*this, AcceptConnection).WillRepeatedly(testing::Invoke([]() {

                std::unique_ptr<NSocket> new_socket_obj = std::make_unique<NSocket>();
                
                int expected_client_fd = 10;
                
                sockaddr_in new_connect;
                new_connect.sin_family = AF_INET;
                new_connect.sin_addr.s_addr = htonl(0x12345678);
                new_connect.sin_port = htons(54321);

                new_socket_obj->set_sock_fd(expected_client_fd);
                new_socket_obj->set_address(new_connect);
            
                return new_socket_obj;
            }));

        EXPECT_CALL(*this, NewConnection(testing::_,testing::_)).WillRepeatedly(testing::Invoke([]() {
                return true;
            }));
    }
    MOCK_METHOD(int, Send, (const std::vector<uint8_t> &data), (override));
    MOCK_METHOD(std::vector<uint8_t>, Receive, (int sbuffer), (override));
    MOCK_METHOD(std::unique_ptr<NSocket>, AcceptConnection, (), (override));
    MOCK_METHOD(bool, NewConnection, (uint16_t port, uint32_t ip), (override));
};



#endif 
