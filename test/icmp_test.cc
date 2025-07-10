#include "include/icmp.h"

#include <iostream> // Include the iostream library for input/output operations

#include <gtest/gtest.h>

class IcmpTest : public ::testing::Test
{
public:
    void SetUp()
    {

    }

protected:
    icmp expect_icmp_;

};


TEST_F(IcmpTest, TestOne){
    std::cout << "Estou aqui nessa\n" ;
    icmp new_icmp = expect_icmp_;
    
    EXPECT_TRUE(new_icmp.teste());
}
