#include "include/nsocket.h"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "test/double/mock_nsocket.h"

class NSocketTest : public ::testing::Test
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
    EchoRequest expect_echo_request_;
    MockNSocket expect_socket_;

    std::vector<uint8_t> message_ = {};
};

TEST_F(NSocketTest, CanCreateSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_NE(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketTest, CanDeleteSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_TRUE(expect_socket_.Close());
    EXPECT_EQ(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketTest, CanSendMessage){

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_NE(expect_socket_.Send(message_, "127.0.0.1"), -1);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketTest, CanReceiveMessage){
    int sbuffer = 20;

    EXPECT_TRUE(expect_socket_.CreateSocket());

    std::vector<uint8_t> received_message = expect_socket_.Receive(sbuffer);
    EXPECT_EQ(received_message, message_);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketTest, CanCreateAddress){
    std::string ip = "0.0.0.0";

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_TRUE(expect_socket_.SetIP(ip));

    EXPECT_TRUE(expect_socket_.Close());
}