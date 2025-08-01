#include "include/nclient.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include "include/echo_reply.h"
#include "include/echo_request.h"
#include "double/mock_nsocket.h"

class NClientTest : public ::testing::Test
{
public:
    void SetUp()
    {
        uint8_t type = 8;
        uint8_t code = 0;
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_type(type);
        expect_echo_request_.set_code(code);
        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> message = expect_echo_request_.Encode();
        message_.reserve(message.size());
        std::copy(message.begin(), message.end(), std::back_inserter(message_));
    }

protected:
    EchoRequest expect_echo_request_;
    std::shared_ptr<MockNSocket> socket_ = std::make_shared<MockNSocket>();
    std::vector<uint8_t> message_ = {};
};


TEST_F(NClientTest, CanInitializeNewSocket) {
    NClient new_client;

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientTest, CanInitializeWithSocket) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientTest, CanPingSomeone) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());
    EXPECT_CALL(*socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReply expect_echo_reply_;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                    0xDE, 0xAD, 0x40, 0x00,
                                    0x40, 0x06, 0x00, 0x00,
                                    0xC0, 0xA8, 0x01, 0x64,
                                    0x08, 0x08, 0x08, 0x08};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    std::string ip_address = "127.0.0.1";
    uint16_t times = 1;
    std::stringstream ss;
    ss << "icmp_seq=" << times-1;
    std::string response = new_client.Ping(ip_address, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address));
    EXPECT_THAT(response, testing::HasSubstr(ss.str()));
}