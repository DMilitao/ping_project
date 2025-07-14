#include "include/icmp.h"

#include <iostream>

#include <gtest/gtest.h>

class IcmpTest : public ::testing::Test
{
public:
    void SetUp()
    {
        uint8_t type = 8;
        uint8_t code = 0;
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;
        std::vector<uint8_t> data = {1,2,3,4};

        expect_icmp_.set_Type(type);
        expect_icmp_.set_Code(code);
        expect_icmp_.set_Identifier(identifier);
        expect_icmp_.set_SequenceNumber(sequence_number);
        expect_icmp_.set_Data(data);
    }

protected:
    icmp expect_icmp_;

};

TEST_F(IcmpTest, CanAccessMessageParameters){
    icmp new_icmp;

    uint8_t type = 3;
    uint8_t code = 3;
    uint16_t identifier = 0x1234;
    uint16_t sequence_number = 0x1234;

    uint8_t pointer = 3;
    uint32_t gateway_address = 0x12345678;
    uint32_t originate_timestamp = 0x12345678;
    uint32_t receive_timestamp = 0x12345678;
    uint32_t transmit_timestamp = 0x12345678;
    std::vector<uint8_t> data = {1,2,3,4,5,6,7};

    EXPECT_NE(new_icmp.Type(),type);
    new_icmp.set_Type(type);
    EXPECT_EQ(new_icmp.Type(),type);

    EXPECT_NE(new_icmp.Code(),code);
    new_icmp.set_Code(code);
    EXPECT_EQ(new_icmp.Code(),code);

    EXPECT_NE(new_icmp.Identifier(),identifier);
    new_icmp.set_Identifier(identifier);
    EXPECT_EQ(new_icmp.Identifier(),identifier);

    EXPECT_NE(new_icmp.SequenceNumber(),sequence_number);
    new_icmp.set_SequenceNumber(sequence_number);
    EXPECT_EQ(new_icmp.SequenceNumber(),sequence_number);

    EXPECT_NE(new_icmp.Pointer(),pointer);
    new_icmp.set_Pointer(pointer);
    EXPECT_EQ(new_icmp.Pointer(),pointer);

    EXPECT_NE(new_icmp.GatewayAddress(),gateway_address);
    new_icmp.set_GatewayAddress(gateway_address);
    EXPECT_EQ(new_icmp.GatewayAddress(),gateway_address);

    EXPECT_NE(new_icmp.OriginateTimeStamp(),originate_timestamp);
    new_icmp.set_OriginateTimeStamp(originate_timestamp);
    EXPECT_EQ(new_icmp.OriginateTimeStamp(),originate_timestamp);

    EXPECT_NE(new_icmp.ReceiveTimeStamp(),receive_timestamp);
    new_icmp.set_ReceiveTimeStamp(receive_timestamp);
    EXPECT_EQ(new_icmp.ReceiveTimeStamp(),receive_timestamp);

    EXPECT_NE(new_icmp.TransmitTimeStamp(),transmit_timestamp);
    new_icmp.set_TransmitTimeStamp(transmit_timestamp);
    EXPECT_EQ(new_icmp.TransmitTimeStamp(),transmit_timestamp);

    EXPECT_NE(new_icmp.Data(),data);
    new_icmp.set_Data(data);
    EXPECT_EQ(new_icmp.Data(),data);
}

TEST_F(IcmpTest, CanEncodeMessage){
    std::vector<uint8_t> message = expect_icmp_.Encode();

    EXPECT_FALSE(message.empty());
}

TEST_F(IcmpTest, CanDecodeMessageEchoRequest){
    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageEchoReply){
    uint8_t type = 0;
    
    expect_icmp_.set_Type(type);

    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageInfoRequest){
    uint8_t type = 15;
    
    expect_icmp_.set_Type(type);
    expect_icmp_.clearData();

    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageInfoReply){
    uint8_t type = 16;
    
    expect_icmp_.set_Type(type);
    expect_icmp_.clearData();
    
    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageDestinationUnreach){
    uint8_t type = 3;
    uint8_t code = 0;

    std::vector<uint8_t> data = {1,2,3,4};

    icmp expect_icmp;

    expect_icmp.set_Type(type);
    expect_icmp.set_Code(code);
    expect_icmp.set_Data(data);

    std::vector<uint8_t> message = expect_icmp.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageTimeExceed){
    uint8_t type = 11;
    uint8_t code = 0;

    std::vector<uint8_t> data = {1,2,3,4};

    icmp expect_icmp;

    expect_icmp.set_Type(type);
    expect_icmp.set_Code(code);
    expect_icmp.set_Data(data);

    std::vector<uint8_t> message = expect_icmp.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageParameterProblem){
    uint8_t type = 12;
    uint8_t code = 0;
    uint8_t pointer = 7;

    std::vector<uint8_t> data = {1,2,3,4};

    icmp expect_icmp;

    expect_icmp.set_Type(type);
    expect_icmp.set_Code(code);
    expect_icmp.set_Pointer(pointer);
    expect_icmp.set_Data(data);

    std::vector<uint8_t> message = expect_icmp.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageSourceQuench){
    uint8_t type = 4;
    uint8_t code = 0;

    std::vector<uint8_t> data = {1,2,3,4};

    icmp expect_icmp;

    expect_icmp.set_Type(type);
    expect_icmp.set_Code(code);
    expect_icmp.set_Data(data);

    std::vector<uint8_t> message = expect_icmp.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageRedirectMessage){
    uint8_t type = 5;
    uint8_t code = 2;
    uint32_t gateway_address = 0xABCD;

    std::vector<uint8_t> data = {1,2,3,4};

    icmp expect_icmp;

    expect_icmp.set_Type(type);
    expect_icmp.set_Code(code);
    expect_icmp.set_GatewayAddress(gateway_address);
    expect_icmp.set_Data(data);

    std::vector<uint8_t> message = expect_icmp.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageTimeStamp){
    uint8_t type = 13;
    uint32_t originate_timestamp = 0xABCD;
    uint32_t receive_timestamp = 0xBCDE;
    uint32_t transmit_timestamp = 0xCDEF;

    expect_icmp_.set_Type(type);
    expect_icmp_.set_OriginateTimeStamp(originate_timestamp);
    expect_icmp_.set_ReceiveTimeStamp(receive_timestamp);
    expect_icmp_.set_TransmitTimeStamp(transmit_timestamp);
    expect_icmp_.clearData();

    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}

TEST_F(IcmpTest, CanDecodeMessageTimeStampReply){
    uint8_t type = 14;
    uint32_t originate_timestamp = 0xABCD;
    uint32_t receive_timestamp = 0xBCDE;
    uint32_t transmit_timestamp = 0xCDEF;

    expect_icmp_.set_Type(type);
    expect_icmp_.set_OriginateTimeStamp(originate_timestamp);
    expect_icmp_.set_ReceiveTimeStamp(receive_timestamp);
    expect_icmp_.set_TransmitTimeStamp(transmit_timestamp);
    expect_icmp_.clearData();

    std::vector<uint8_t> message = expect_icmp_.Encode();

    icmp new_icmp;

    EXPECT_TRUE(new_icmp.Decode(message));
    
    EXPECT_TRUE(new_icmp == expect_icmp_);

    EXPECT_EQ(new_icmp.Encode(),message);
}