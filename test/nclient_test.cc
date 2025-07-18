#include "include/nclient.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test/double/mock_nclient.h"

class NClientTest : public ::testing::Test
{
public:
    void SetUp()
    {

    }

protected:

};

TEST_F(NClientTest, CanCreateConnection){
    MockNClient expect_client_;

    EXPECT_TRUE(expect_client_.ConnectToServer(8080,0));
}

TEST_F(NClientTest, CanCloseConnection){
    MockNClient expect_client_;

    expect_client_.CreateSocket();

    EXPECT_TRUE(expect_client_.DisconnectFromServer());
}