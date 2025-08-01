#include "include/nserver.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/echo_request.h"
#include "include/echo_reply.h"
#include "double/mock_nsocket.h"
class NServerTest : public ::testing::Test
{
public:
    void SetUp()
    {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);
    }

protected:
    EchoRequest expect_echo_request_;
    std::string ip_address_ = "127.0.0.1";
    std::shared_ptr<MockNSocket> socket_ = std::make_shared<MockNSocket>();

};

TEST_F(NServerTest, CanInitializeNewSocket){
    NServer expect_server_(ip_address_);

    EXPECT_TRUE(expect_server_.isOpen());
}

TEST_F(NServerTest, CanInitializeWithSocket) {
    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());
}

TEST_F(NServerTest, CanReceiveMessage) {
    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());
    EXPECT_CALL(*socket_, Receive(testing::_)).WillOnce(testing::Invoke([](const int sbuffer) {
        std::vector<uint8_t> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        uint16_t identifier = 0xABCD;
        uint16_t sequence_number = 0xCDEF;
        EchoRequest expect_echo_request_;

        expect_echo_request_.set_data(data);
        expect_echo_request_.set_identifier(identifier);
        expect_echo_request_.set_sequence_number(sequence_number);

        std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

        std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                    0xDE, 0xAD, 0x40, 0x00,
                                    0x40, 0x06, 0x00, 0x00,
                                    0xC0, 0xA8, 0x01, 0x64,
                                    0x08, 0x08, 0x08, 0x08};
        msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

        if (msg.size() > sbuffer){
            msg.resize(sbuffer);
        }

        return msg;
    }));

    std::vector<uint8_t> received_message = new_server.Receive(1024);
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                 0xC0, 0xA8, 0x01, 0x64,
                                 0x08, 0x08, 0x08, 0x08};
    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    EXPECT_EQ(received_message, msg);
}

TEST_F(NServerTest, CanHandleMessage) {
    NServer new_server(ip_address_, socket_);

    EXPECT_TRUE(new_server.isOpen());
    std::vector<uint8_t> icmp_msg = expect_echo_request_.Encode();

    std::string ip_from = "192.168.1.100";
    std::vector<uint8_t> msg = { 0x45, 0x00, 0x00, 0x28,
                                 0xDE, 0xAD, 0x40, 0x00,
                                 0x40, 0x06, 0x00, 0x00,
                                  192,  168,    1,  100,
                                 0x08, 0x08, 0x08, 0x08};

    msg.insert(msg.end(),icmp_msg.begin(),icmp_msg.end());

    std::string response = new_server.HandleMessage(msg);
    EXPECT_THAT(response, testing::HasSubstr(ip_from));
}