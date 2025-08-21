#include "include/nserver.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "include/echo_reply.h"
#include "include/echo_request_ipv6.h"
#include "include/echo_reply_ipv6.h"
#include "double/mock_nsocket_ipv4.h"
#include "double/mock_nsocket_ipv6.h"

class NServerIPv4Test : public ::testing::Test
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

        EXPECT_CALL(*socket_, last_ip_com()).WillRepeatedly(testing::Invoke([]() {
                std::string ip_address = "127.0.0.1";
            return ip_address;
        }));
    }

protected:
    EchoRequest expect_echo_request_;

    std::string ip_address_ = "127.0.0.5";
    std::string ip_from_ = "127.0.0.1";

    std::shared_ptr<MockNSocketIPv4> socket_ = std::make_shared<MockNSocketIPv4>();

};

TEST_F(NServerIPv4Test, CanInitializeNewSocket){
    NServer expect_server_(ip_address_);

    EXPECT_TRUE(expect_server_.isOpen());
}

TEST_F(NServerIPv4Test, CanInitializeWithSocket) {
    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());
}

TEST_F(NServerIPv4Test, CanReceiveMessage) {
    EXPECT_CALL(*socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
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

    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());

    std::vector<uint8_t> received_message = new_server.Receive(1024);
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x40, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};
    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EXPECT_EQ(received_message, msg);
}

TEST_F(NServerIPv4Test, CanHandleMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x40, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                  192,  168,    1,  100,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());
    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    
    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());

    std::string response = new_server.HandleMessage(msg);

    EXPECT_THAT(response, testing::HasSubstr("EchoRequest"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
    EXPECT_THAT(response, testing::Not(testing::HasSubstr("error")));
    EXPECT_THAT(response, testing::HasSubstr(ss.str()));
}

TEST_F(NServerIPv4Test, CannotAnswerMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x40, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                  192,  168,    1,  100,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    std::string response = new_server.HandleMessage(msg);

    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    EXPECT_THAT(response, testing::HasSubstr("EchoRequest"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
    EXPECT_THAT(response, testing::HasSubstr("error"));
    EXPECT_THAT(response, testing::Not(testing::HasSubstr(ss.str())));
}

TEST_F(NServerIPv4Test, CannotIdentifyTypeOfMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(0));

    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());

    EchoReply new_echo_reply;
    new_echo_reply.set_identifier(expect_echo_request_.identifier());
    new_echo_reply.set_sequence_number(expect_echo_request_.sequence_number());
    new_echo_reply.set_data(expect_echo_request_.data());

    std::vector<uint8_t> icmp_msg = new_echo_reply.Encode();

    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x40, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                  192,  168,    1,  100,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    std::string response = new_server.HandleMessage(msg);

    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    EXPECT_THAT(response, testing::HasSubstr("Unidentified message"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
}

class NServerIPv6Test : public ::testing::Test
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
        expect_echo_request_.set_ip_source(ip_from_);
        expect_echo_request_.set_ip_destiny(ip_address_);

        EXPECT_CALL(*socket_, last_ip_com()).WillRepeatedly(testing::Invoke([]() {
                std::string ip_address = "::1";
            return ip_address;
        }));
    }

protected:
    EchoRequestIPv6 expect_echo_request_;

    std::string ip_address_ = "::1";
    std::string ip_from_ = "::1";

    std::shared_ptr<MockNSocketIPv6> socket_ = std::make_shared<MockNSocketIPv6>();

};

TEST_F(NServerIPv6Test, CanInitializeNewSocket){
    NServer expect_server_(ip_address_, "ipv6");

    EXPECT_TRUE(expect_server_.isOpen());
}

TEST_F(NServerIPv6Test, CanInitializeWithSocket) {
    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());
}

TEST_F(NServerIPv6Test, CanReceiveMessage) {
    EXPECT_CALL(*socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;
        EchoRequestIPv6 expect_echo_request_;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
        expect_echo_request_.set_ip_source("::1");
        expect_echo_request_.set_ip_destiny("::1");

        std::vector<uint8_t> msg = expect_echo_request_.Encode();

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());

    std::vector<uint8_t> received_message = new_server.Receive(4096);
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    EXPECT_EQ(received_message, msg);
}

TEST_F(NServerIPv6Test, CanHandleMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return (int)data_arg.size();
    }));

    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    
    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());

    std::string response = new_server.HandleMessage(msg);

    EXPECT_THAT(response, testing::HasSubstr("EchoRequest"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
    EXPECT_THAT(response, testing::Not(testing::HasSubstr("error")));
    EXPECT_THAT(response, testing::HasSubstr(ss.str()));
}

TEST_F(NServerIPv6Test, CannotAnswerMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).WillRepeatedly(testing::Invoke([](const std::vector<uint8_t>& data_arg, const std::string ip) {
        return -1;
    }));

    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    std::string response = new_server.HandleMessage(msg);

    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    EXPECT_THAT(response, testing::HasSubstr("EchoRequest"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
    EXPECT_THAT(response, testing::HasSubstr("error"));
    EXPECT_THAT(response, testing::Not(testing::HasSubstr(ss.str())));
}

TEST_F(NServerIPv6Test, CannotIdentifyTypeOfMessage) {
    EXPECT_CALL(*socket_, Send(testing::A<const std::vector<uint8_t> &>(), testing::_)).Times(testing::Exactly(0));

    NServer new_server(ip_address_, socket_);
    EXPECT_TRUE(new_server.isOpen());

    EchoReply new_echo_reply;
    new_echo_reply.set_identifier(expect_echo_request_.identifier());
    new_echo_reply.set_sequence_number(expect_echo_request_.sequence_number());
    new_echo_reply.set_data(expect_echo_request_.data());

    std::vector<uint8_t> msg = new_echo_reply.Encode();

    std::string response = new_server.HandleMessage(msg);

    std::stringstream ss;
    ss << expect_echo_request_.Encode().size() << " bytes";

    EXPECT_THAT(response, testing::HasSubstr("Unidentified message"));
    EXPECT_THAT(response, testing::HasSubstr(ip_from_));
}