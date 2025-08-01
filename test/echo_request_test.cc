#include "include/echo_request.h"

#include <gtest/gtest.h>

#include "include/echo_reply.h"
class EchoRequestTest : public ::testing::Test
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
    }

protected:
    EchoRequest expect_echo_request_;
};

TEST_F(EchoRequestTest, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_request_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoRequestTest, CanDecodeMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EchoRequest new_echo_request;

    EXPECT_TRUE(new_echo_request.Decode(msg));

    EXPECT_TRUE(new_echo_request == expect_echo_request_);

    EXPECT_EQ(new_echo_request.Encode(),icmp_msg);
}

TEST_F(EchoRequestTest, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

    icmp_msg[0] = 0;

    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EchoRequest new_echo_request;

    EXPECT_FALSE(new_echo_request.Decode(msg));
}

TEST_F(EchoRequestTest, CanIdentifyBrokenMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoRequest new_echo_request;

    EXPECT_TRUE(new_echo_request.Decode(message_broken));

    EXPECT_FALSE(new_echo_request.checksum() == 0);
}

TEST_F(EchoRequestTest, CanCompareToEchoReply){
    std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    uint16_t identifier = 0xABCD;
    uint16_t sequence_number = 0xCDEF;

    EchoReply new_echo_reply;

    new_echo_reply.set_data(data);
    new_echo_reply.set_identifier(identifier);
    new_echo_reply.set_sequence_number(sequence_number);

    EXPECT_TRUE(expect_echo_request_ == new_echo_reply);
}