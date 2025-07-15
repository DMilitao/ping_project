#include "include/icmp.h"

#include <gtest/gtest.h>

class IcmpTest : public ::testing::Test
{
public:
    void SetUp()
    {


    }

protected:
    Icmp expect_icmp_;
    std::vector<uint8_t> message_ = {2,  // Type
                                     0,  // Code
                                     0,  // First byte check sum
                                     0,  // Second byte check sum
                                     0,  // unused
                                     0,  // unused
                                     0,  // unused
                                     0}; // unused
};

TEST_F(IcmpTest, CanCreateCheckSumByte){
    uint16_t checksum = expect_icmp_.createCheckSum(message_);
    
    EXPECT_FALSE(expect_icmp_.verifyCheckSum(message_));

    message_.at(2) = (checksum >> 8) & 0xFF;
    message_.at(3) = (checksum) & 0xFF;
    
    EXPECT_TRUE(expect_icmp_.verifyCheckSum(message_));
}

TEST_F(IcmpTest, CanVerifyErrorInReceivedMessage){
    uint16_t checksum = expect_icmp_.createCheckSum(message_);

    message_.at(2) = (checksum >> 8) & 0xFF;
    message_.at(3) = (checksum) & 0xFF;
    
    EXPECT_TRUE(expect_icmp_.verifyCheckSum(message_));

    message_.at(4) = 0xA1;

    EXPECT_FALSE(expect_icmp_.verifyCheckSum(message_));
}