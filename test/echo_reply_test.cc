#include "include/echo_reply.h"

#include <gtest/gtest.h>

class EchoReplyTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4};
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
    std::vector<uint8_t> message = expect_echo_reply_.Encode();

    EXPECT_FALSE(message.empty());
}

TEST_F(EchoReplyTest, CanDecodeMessage){
    std::vector<uint8_t> message = expect_echo_reply_.Encode();

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(message));

    EXPECT_TRUE(new_echo_reply == expect_echo_reply_);

    EXPECT_EQ(new_echo_reply.Encode(),message);
}

TEST_F(EchoReplyTest, CannotDecodeOtherTypeMessage){
    std::vector<uint8_t> message = expect_echo_reply_.Encode();

    EchoReply new_echo_reply;
    
    message[0] = 8;

    EXPECT_FALSE(new_echo_reply.Decode(message));
}

TEST_F(EchoReplyTest, CanIdentifyBrokenMessage){
    std::vector<uint8_t> message = expect_echo_reply_.Encode();

    std::vector<uint8_t> message_broken(message.begin(),message.end()-1);

    EchoReply new_echo_reply;

    EXPECT_TRUE(new_echo_reply.Decode(message_broken));
    
    EXPECT_FALSE(new_echo_reply.checksum() == 0);
}