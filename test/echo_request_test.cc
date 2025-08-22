#include "include/echo_request.h"
#include "include/echo_request_ipv6.h"

#include <gtest/gtest.h>

#include "include/echo_reply.h"
#include "include/echo_reply_ipv6.h"
class EchoRequestTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_type(expect_echo_request_.type_default());
        expect_echo_request_.set_code(expect_echo_request_.code_default());
        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
    }

protected:
    EchoRequest expect_echo_request_;
    std::vector<uint8_t> generic_internet_header = {0x45, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x40, 0x00,
                                                    0x00, 0x00, 0x00, 0x00,
                                                    127,     0,    0,    1,
                                                    127,     0,    0,    1};
};

TEST_F(EchoRequestTest, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoRequestTest, CanDecodeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    EchoRequest new_echo_request;

    EXPECT_TRUE(new_echo_request.Decode(msg));

    EXPECT_EQ(new_echo_request.isEqual(expect_echo_request_), 1);

    EXPECT_EQ(new_echo_request.checksum(), 0);
}

TEST_F(EchoRequestTest, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    msg[0] = 0;

    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    EchoRequest new_echo_request;

    EXPECT_FALSE(new_echo_request.Decode(msg));
}

TEST_F(EchoRequestTest, CanIdentifyBrokenMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();
    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoRequest new_echo_request;

    EXPECT_TRUE(new_echo_request.Decode(message_broken));

    EXPECT_NE(new_echo_request.checksum(), 0);
}

TEST_F(EchoRequestTest, CanCompareToEchoReply){
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uint16_t identifier = 0xABCD;
    uint16_t sequence_number = 0xCDEF;

    EchoReply new_echo_reply;

    new_echo_reply.set_type(new_echo_reply.type_default());
    new_echo_reply.set_code(new_echo_reply.code_default());
    new_echo_reply.set_data(data);
    new_echo_reply.set_identifier(identifier);
    new_echo_reply.set_sequence_number(sequence_number);

    EXPECT_EQ(new_echo_reply.isEqual(expect_echo_request_), 2);
}

class EchoRequestIPv6Test : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_ip_source(ip_address_);
        expect_echo_request_.set_ip_destiny(ip_address_);

        expect_echo_request_.set_type(expect_echo_request_.type_default());
        expect_echo_request_.set_code(expect_echo_request_.code_default());
        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
    }

protected:
    EchoRequestIPv6 expect_echo_request_;
    std::string ip_address_ = "::1";
};

TEST_F(EchoRequestIPv6Test, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoRequestIPv6Test, CanDecodeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    EchoRequestIPv6 new_echo_request;

    new_echo_request.set_ip_source(ip_address_);
    new_echo_request.set_ip_destiny(ip_address_);

    EXPECT_TRUE(new_echo_request.Decode(msg));

    EXPECT_EQ(new_echo_request.isEqual(expect_echo_request_), 1);
    
    EXPECT_EQ(new_echo_request.checksum(), 0);
}

TEST_F(EchoRequestIPv6Test, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    msg[0] = 129;

    EchoRequestIPv6 new_echo_request;
    new_echo_request.set_ip_source(ip_address_);
    new_echo_request.set_ip_destiny(ip_address_);

    EXPECT_FALSE(new_echo_request.Decode(msg));
}

TEST_F(EchoRequestIPv6Test, CanIdentifyBrokenMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoRequestIPv6 new_echo_request;

    new_echo_request.set_ip_source(ip_address_);
    new_echo_request.set_ip_destiny(ip_address_);

    EXPECT_TRUE(new_echo_request.Decode(message_broken));

    EXPECT_NE(new_echo_request.checksum(),0);
}

TEST_F(EchoRequestIPv6Test, CanCompareToEchoReply){
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uint16_t identifier = 0xABCD;
    uint16_t sequence_number = 0xCDEF;

    EchoReplyIPv6 new_echo_reply;

    new_echo_reply.set_type(new_echo_reply.type_default());
    new_echo_reply.set_code(new_echo_reply.code_default());
    new_echo_reply.set_data(data);
    new_echo_reply.set_identifier(identifier);
    new_echo_reply.set_sequence_number(sequence_number);

    EXPECT_EQ(new_echo_reply.isEqual(expect_echo_request_), 2);
}