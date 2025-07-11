#include "include/icmp.h"

#include <iostream>

#include <gtest/gtest.h>

class IcmpTest : public ::testing::Test
{
public:
    void SetUp()
    {
        expect_icmp_.set_Type(0);
        expect_icmp_.set_Code(0);
    }

protected:
    icmp expect_icmp_;

    // Just for initial tests while the Encode() function is not implemented yet
    std::vector<uint8_t> message_ = {8, // Type
                                     0, // Code
                                     0, // First byte CheckSum
                                     0, // Second byte CheckSum
                                     0, // First Byte Identifier
                                     0, // Second Byte Identifier
                                     0, // First Byte Sequence Number
                                     0, // Second Byte Sequence Number
                                     };
};


TEST_F(IcmpTest, CanIdentifyMessageType){
    icmp new_icmp;   

    EXPECT_TRUE(new_icmp.Decode(message_));
}
