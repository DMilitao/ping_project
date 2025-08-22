#include "include/echo_reply.h"
#include "include/echo_reply_ipv6.h"

#include <gtest/gtest.h>

class EchoReplyTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_reply_.set_type(expect_echo_reply_.type_default());
        expect_echo_reply_.set_code(expect_echo_reply_.code_default());
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);
    }

protected:
    EchoReply expect_echo_reply_;
    std::vector<uint8_t> generic_internet_header = {0x45, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x40, 0x00,
                                                    0x00, 0x00, 0x00, 0x00,
                                                    127,     0,    0,    1,
                                                    127,     0,    0,    1};
};

TEST_F(EchoReplyTest, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoReplyTest, CanDecodeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(msg));

    EXPECT_EQ(new_echo_reply.isEqual(expect_echo_reply_), 1);

    EXPECT_EQ(new_echo_reply.checksum(), 0);
}

TEST_F(EchoReplyTest, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    msg[0] = 8;

    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    EchoReply new_echo_reply;

    EXPECT_FALSE(new_echo_reply.Decode(msg));
}

TEST_F(EchoReplyTest, CanIdentifyBrokenMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    msg.insert(msg.begin(),generic_internet_header.begin(),generic_internet_header.end());

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(message_broken));

    EXPECT_NE(new_echo_reply.checksum(), 0);
}

class EchoReplyIPv6Test : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_reply_.set_ip_source(ip_address_);
        expect_echo_reply_.set_ip_destiny(ip_address_);

        expect_echo_reply_.set_type(expect_echo_reply_.type_default());
        expect_echo_reply_.set_code(expect_echo_reply_.code_default());
        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);
    }

protected:
    EchoReplyIPv6 expect_echo_reply_;
    std::string ip_address_ = "::1";
};

TEST_F(EchoReplyIPv6Test, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoReplyIPv6Test, CanDecodeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    EchoReplyIPv6 new_echo_reply;
    
    new_echo_reply.set_ip_source(ip_address_);
    new_echo_reply.set_ip_destiny(ip_address_);

    EXPECT_TRUE(new_echo_reply.Decode(msg));

    EXPECT_EQ(new_echo_reply.isEqual(expect_echo_reply_), 1);

    EXPECT_EQ(new_echo_reply.checksum(), 0);
}

TEST_F(EchoReplyIPv6Test, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    msg[0] = 8;

    EchoReplyIPv6 new_echo_reply;

    new_echo_reply.set_ip_source(ip_address_);
    new_echo_reply.set_ip_destiny(ip_address_);

    EXPECT_FALSE(new_echo_reply.Decode(msg));
}

TEST_F(EchoReplyIPv6Test, CanIdentifyBrokenMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoReplyIPv6 new_echo_reply;
    new_echo_reply.set_ip_source(ip_address_);
    new_echo_reply.set_ip_destiny(ip_address_);

    EXPECT_TRUE(new_echo_reply.Decode(message_broken));

    EXPECT_NE(new_echo_reply.checksum(), 0);
}