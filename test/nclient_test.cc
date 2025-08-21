#include "include/nclient.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

#include "double/mock_nsocket_ipv4.h"
#include "double/mock_nsocket_ipv6.h"
#include "include/echo_reply.h"
#include "include/echo_reply_ipv6.h"
#include "include/echo_request.h"
#include "include/echo_request_ipv6.h"

class NClientIPv4 : public ::testing::Test
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

        EXPECT_CALL(*socket_, last_ip_com()).WillRepeatedly(testing::Invoke([]() {
                std::string ip_address = "127.0.0.1";
            return ip_address;
        }));
    }

protected:
    EchoRequest expect_echo_request_;
    std::shared_ptr<MockNSocketIPv4> socket_ = std::make_shared<MockNSocketIPv4>();
    std::vector<uint8_t> message_ = {};
    std::string ip_address_ = "127.0.0.1";
};

TEST_F(NClientIPv4, CanInitializeNewSocket) {
    NClient new_client;

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientIPv4, CanInitializeWithSocket) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientIPv4, CanHandleTimesNegative) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());

    int times = -1;

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_TRUE(response.empty());
}

TEST_F(NClientIPv4, CanPingSomeone) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReply expect_echo_reply_;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                      127,    0,    0,    1,
                                      127,    0,    0,    1};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());


    std::stringstream ss;
    ss << "icmp_seq=" << times-1;
    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr(ss.str()));
}

TEST_F(NClientIPv4, CannotSendPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_TRUE(response.empty());
}

TEST_F(NClientIPv4, CanDetectWrongTypeOfAnswerInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoRequest expect_echo_request_;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                      127,    0,    0,    1,
                                      127,    0,    0,    1};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv4, CanDetectBrokenMessageDueToWrongHeaderInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = getpid()+1;
        uint16_t sequence_number = 1;
        EchoReply expect_echo_reply_;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                      127,    0,    0,    1,
                                      127,    0,    0,    1};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv4, CanDetectBrokenMessageDueToDataChangeInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReply expect_echo_reply_;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                      127,    0,    0,    1,
                                      127,    0,    0,    1};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv4, CanDetectBrokenMessaDueToChecksumErrorInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReply expect_echo_reply_;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x40, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                      127,    0,    0,    1,
                                      127,    0,    0,    1};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }
        //Adding 20 at the end to correspond data, but to break the checksum
        msg.push_back(20);

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv4, CanDetectTimeoutInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> msg = {};
        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("time=Timeout"));
}

class NClientIPv6 : public ::testing::Test
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

        EXPECT_CALL(*socket_, last_ip_com()).WillRepeatedly(testing::Invoke([]() {
            std::string ip_address = "::1";
            return ip_address;
        }));
    }

protected:
    EchoRequest expect_echo_request_;
    std::shared_ptr<MockNSocketIPv6> socket_ = std::make_shared<MockNSocketIPv6>();
    std::vector<uint8_t> message_ = {};
    std::string ip_address_ = "::1";
};

TEST_F(NClientIPv6, CanInitializeNewSocket) {
    NClient new_client;

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientIPv6, CanInitializeWithSocket) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());
}

TEST_F(NClientIPv6, CanHandleTimesNegative) {
    NClient new_client(socket_);

    EXPECT_TRUE(new_client.isOpen());

    int times = -1;

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_TRUE(response.empty());
}

TEST_F(NClientIPv6, CanPingSomeone) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReplyIPv6 expect_echo_reply_;

        std::string ip_address = "::1";
        expect_echo_reply_.set_ip_source(ip_address);
        expect_echo_reply_.set_ip_destiny(ip_address);
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> msg = expect_echo_reply_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());


    std::stringstream ss;
    ss << "icmp_seq=" << times-1;
    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr(ss.str()));
}

TEST_F(NClientIPv6, CannotSendPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_TRUE(response.empty());
}

TEST_F(NClientIPv6, CanDetectWrongTypeOfAnswerInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoRequestIPv6 expect_echo_request_;

        std::string ip_address = "::1";
        expect_echo_request_.set_ip_source(ip_address);
        expect_echo_request_.set_ip_destiny(ip_address);
        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> msg = expect_echo_request_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv6, CanDetectBrokenMessageDueToWrongHeaderInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = getpid()+1;
        uint16_t sequence_number = 1;
        EchoReplyIPv6 expect_echo_reply_;

        std::string ip_address = "::1";
        expect_echo_reply_.set_ip_source(ip_address);
        expect_echo_reply_.set_ip_destiny(ip_address);
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> msg = expect_echo_reply_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv6, CanDetectBrokenMessageDueToDataChangeInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReplyIPv6 expect_echo_reply_;

        std::string ip_address = "::1";
        expect_echo_reply_.set_ip_source(ip_address);
        expect_echo_reply_.set_ip_destiny(ip_address);
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> msg = expect_echo_reply_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv6, CanDetectBrokenMessaDueToChecksumErrorInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

        uint16_t identifier = getpid();
        uint16_t sequence_number = 0;
        EchoReplyIPv6 expect_echo_reply_;

        std::string ip_address = "::1";
        expect_echo_reply_.set_ip_source(ip_address);
        expect_echo_reply_.set_ip_destiny(ip_address);
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);

        std::vector<uint8_t> msg = expect_echo_reply_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }
        //Adding 20 at the end to correspond data, but to break the checksum
        msg.push_back(20);

        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("broken"));
}

TEST_F(NClientIPv6, CanDetectTimeoutInPing) {
    int times = 1;

    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    EXPECT_CALL(*socket_, Receive(testing::_)).Times(testing::Exactly(times)).WillRepeatedly(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> msg = {};
        return msg;
    }));

    NClient new_client(socket_);
    EXPECT_TRUE(new_client.isOpen());

    std::string response = new_client.Ping(ip_address_, times);

    EXPECT_THAT(response, testing::HasSubstr(ip_address_));
    EXPECT_THAT(response, testing::HasSubstr("time=Timeout"));
}