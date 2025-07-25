#include "include/nclient.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "test/double/mock_nclient.h"

class NClientTest : public ::testing::Test
{
public:
    void SetUp()
    {
        uint8_t type = 8;
        uint8_t code = 0;
        std::vector<uint8_t> data = {1, 2, 3, 4};

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
    MockNClient expect_client_;
    std::string ip_address_ = "0.0.0.0";
    EchoRequest expect_echo_request_;

    std::vector<uint8_t> message_ = {};
};


TEST_F(NClientTest, CanOpenClient) {
    EXPECT_TRUE(expect_client_.Open(ip_address_));
    EXPECT_TRUE(expect_client_.isOpen());
}

TEST_F(NClientTest, CanCloseClient) {
    EXPECT_TRUE(expect_client_.Open(ip_address_));
    EXPECT_TRUE(expect_client_.Close());
    EXPECT_FALSE(expect_client_.isOpen());
}

TEST_F(NClientTest, CanSendMessage){

    EXPECT_TRUE(expect_client_.Open(ip_address_));

    EXPECT_NE(expect_client_.Send(message_, "127.0.0.1"), -1);

    EXPECT_TRUE(expect_client_.Close());
}

TEST_F(NClientTest, CanReceiveMessage){
    int sbuffer = 20;

    EXPECT_TRUE(expect_client_.Open(ip_address_));

    std::vector<uint8_t> received_message = expect_client_.WaitingMessage(sbuffer);
    EXPECT_EQ(received_message, message_);

    EXPECT_TRUE(expect_client_.Close());
}