#include <gmock/gmock.h>

#include "include/nclient.h"

#ifndef PING_PROJECT_INCLUDE_MOCK_NCLIENT_H_
#define PING_PROJECT_INCLUDE_MOCK_NCLIENT_H_

class MockNClient : public NClient {
   public:

    MockNClient() : NClient() {

        EXPECT_CALL(*this, ConnectToServer(testing::_,testing::_)).WillRepeatedly(testing::Invoke([]() {
                return true;
            }));
    }

    MOCK_METHOD(bool, ConnectToServer, (uint16_t port, uint32_t ip), (override));
};



#endif 
