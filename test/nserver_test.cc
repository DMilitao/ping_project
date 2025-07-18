#include "include/nserver.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"


class NServerTest : public ::testing::Test
{

};

TEST_F(NServerTest, CanCreateAndCloseServer){
    NServer expect_server_;
    EXPECT_FALSE(expect_server_.Close());
    EXPECT_TRUE(expect_server_.StartServer(8080));
    EXPECT_TRUE(expect_server_.Close());
}

