#include "include/nsocket_ipv4.h"
#include "include/nsocket_ipv6.h"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "include/echo_request_ipv6.h"
#include "test/double/mock_nsocket_ipv4.h"
#include "test/double/mock_nsocket_ipv6.h"

class NSocketIPv4Test : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> message_ = expect_echo_request_.Encode();
    }

protected:
    EchoRequest expect_echo_request_;
    MockNSocketIPv4 expect_socket_;

    std::string ip_address_ = "127.0.0.1";
    std::vector<uint8_t> message_ = {};
};

TEST_F(NSocketIPv4Test, CanCreateSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_NE(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketIPv4Test, CanDeleteSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_TRUE(expect_socket_.Close());
    EXPECT_EQ(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketIPv4Test, CanSendMessage){
    EXPECT_CALL(expect_socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_NE(expect_socket_.Send(message_, ip_address_), -1);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv4Test, CannotSendMessage){
    EXPECT_CALL(expect_socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_EQ(expect_socket_.Send(message_, ip_address_), -1);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv4Test, CanReceiveMessage){
    int sbuffer = 1024;

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_CALL(expect_socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;
        EchoRequest expect_echo_request_;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0xC0, 0xA8, 0x01, 0x64,
                                     0x08, 0x08, 0x08, 0x08};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    std::vector<uint8_t> received_message = expect_socket_.Receive(sbuffer);

    EXPECT_FALSE(received_message.empty());

    EchoRequest echo_request_received;
    EXPECT_TRUE(echo_request_received.Decode(received_message));
    EXPECT_EQ(echo_request_received, expect_echo_request_);
    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv4Test, CannotReceiveMessage){
    int sbuffer = 1024;

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_CALL(expect_socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> msg = {};
        return msg;
    }));

    std::vector<uint8_t> received_message = expect_socket_.Receive(sbuffer);

    EXPECT_TRUE(received_message.empty());

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv4Test, CanCreateAddress){
    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_TRUE(expect_socket_.SetIP(ip_address_));

    EXPECT_TRUE(expect_socket_.Close());
}

class NSocketIPv6Test : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
        expect_echo_request_.set_ip_source(ip_address_);
        expect_echo_request_.set_ip_destiny(ip_address_);
        message_ = expect_echo_request_.Encode();
    }

protected:
    EchoRequestIPv6 expect_echo_request_;
    MockNSocketIPv6 expect_socket_;
    std::string ip_address_ = "::1";
    std::vector<uint8_t> message_ = {};
};

TEST_F(NSocketIPv6Test, CanCreateSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_NE(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketIPv6Test, CanDeleteSocket){
    EXPECT_TRUE(expect_socket_.CreateSocket());
    EXPECT_TRUE(expect_socket_.Close());
    EXPECT_EQ(expect_socket_.sock_fd(), -1);
}

TEST_F(NSocketIPv6Test, CanSendMessage){
    EXPECT_CALL(expect_socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_NE(expect_socket_.Send(message_, ip_address_), -1);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv6Test, CannotSendMessage){
    EXPECT_CALL(expect_socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_EQ(expect_socket_.Send(message_, ip_address_), -1);

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv6Test, CanReceiveMessage){
    int sbuffer = 4096;

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_CALL(expect_socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        std::string ip_address_ = "::1";
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        EchoRequestIPv6 expect_echo_request_;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
        expect_echo_request_.set_ip_source(ip_address_);
        expect_echo_request_.set_ip_destiny(ip_address_);

        std::vector<uint8_t> msg = expect_echo_request_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    std::vector<uint8_t> received_message = expect_socket_.Receive(sbuffer);

    EXPECT_FALSE(received_message.empty());

    EchoRequestIPv6 echo_request_received;
    EXPECT_TRUE(echo_request_received.Decode(received_message));
    EXPECT_EQ(echo_request_received, expect_echo_request_);
    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv6Test, CannotReceiveMessage){
    int sbuffer = 1024;

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_CALL(expect_socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> msg = {};
        return msg;
    }));

    std::vector<uint8_t> received_message = expect_socket_.Receive(sbuffer);

    EXPECT_TRUE(received_message.empty());

    EXPECT_TRUE(expect_socket_.Close());
}

TEST_F(NSocketIPv6Test, CanCreateAddress){
    std::string ip = "::1";

    EXPECT_TRUE(expect_socket_.CreateSocket());

    EXPECT_TRUE(expect_socket_.SetIP(ip));

    EXPECT_TRUE(expect_socket_.Close());
}