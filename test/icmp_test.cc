#include "include/icmp.h"

#include <iostream> // Include the iostream library for input/output operations

#include <gtest/gtest.h>

class IcmpTest : public ::testing::Test
{
public:
    void SetUp()
    {

    }

private:
    icmp expect_icmp_;

};


TEST_F(IcmpTest, TestOne){
    cout << "Estou aqui" ;
    icmp new_icmp = expect_icmp_;
    
    EXPECT_FALSE(icmp.teste());
}
