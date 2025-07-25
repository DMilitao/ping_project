#include "include/nserver.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "include/echo_reply.h"
#include "test/double/mock_nserver.h"

class NServerTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> message = expect_echo_request_.Encode();
        message_.reserve(message.size());
        std::copy(message.begin(), message.end(), std::back_inserter(message_));
    }

protected:
    MockNServer expect_server_;
    std::string ip_address_ = "0.0.0.0";
    EchoRequest expect_echo_request_;

    std::vector<uint8_t> message_ = {};
};


TEST_F(NServerTest, CanOpenServer) {
    EXPECT_TRUE(expect_server_.Open(ip_address_));
    EXPECT_TRUE(expect_server_.isOpen());
}

TEST_F(NServerTest, CanCloseServer) {
    EXPECT_TRUE(expect_server_.Open(ip_address_));
    EXPECT_TRUE(expect_server_.Close());
    EXPECT_FALSE(expect_server_.isOpen());
}

TEST_F(NServerTest, CanSendMessage){

    EXPECT_TRUE(expect_server_.Open(ip_address_));

    EXPECT_NE(expect_server_.Send(message_, "127.0.0.1"), -1);

    EXPECT_TRUE(expect_server_.Close());
}

TEST_F(NServerTest, CanReceiveMessage){
    int sbuffer = 20;

    EXPECT_TRUE(expect_server_.Open(ip_address_));

    std::vector<uint8_t> received_message = expect_server_.WaitingMessage(sbuffer);
    EXPECT_EQ(received_message, message_);

    EXPECT_TRUE(expect_server_.Close());
}

TEST_F(NServerTest, CanHandleMessage){
    EXPECT_TRUE(expect_server_.Open(ip_address_));

    EXPECT_NE(expect_server_.HandleMessage(message_), -1);

    EXPECT_TRUE(expect_server_.Close());
}