#include "include/echo_reply.h"

#include <gtest/gtest.h>

class EchoReplyTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_reply_.set_data(data);
        expect_echo_reply_.set_identifier(identifier);
        expect_echo_reply_.set_sequence_number(sequence_number);
    }

protected:
    EchoReply expect_echo_reply_;
};

TEST_F(EchoReplyTest, CanEncodeMessage){
    std::vector<uint8_t> msg = expect_echo_reply_.Encode();

    EXPECT_FALSE(msg.empty());
}

TEST_F(EchoReplyTest, CanDecodeMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};
    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(msg));

    EXPECT_TRUE(new_echo_reply == expect_echo_reply_);

    EXPECT_EQ(new_echo_reply.Encode(),icmp_msg);
}

TEST_F(EchoReplyTest, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();

    icmp_msg[0] = 8;

    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EchoReply new_echo_reply;

    EXPECT_FALSE(new_echo_reply.Decode(msg));
}

TEST_F(EchoReplyTest, CanIdentifyBrokenMessage){
    std::vector<uint8_t> icmp_msg = expect_echo_reply_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    std::vector<uint8_t> message_broken(msg.begin(),msg.end()-1);

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(message_broken));

    EXPECT_FALSE(new_echo_reply.checksum() == 0);
}