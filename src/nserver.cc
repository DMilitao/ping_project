/**
 * \file
 * \brief NServer class definition
 */
#include "include/nserver.h"

#include "include/echo_request.h"
#include "include/echo_reply.h"

bool NServer::Open(const std::string ip_address){
    if ( isOpen() ){
        return false;
    }

    return socket_.CreateSocket() && socket_.SetIP(ip_address);
}

bool NServer::isOpen() const {
    return socket_.sock_fd() != -1;
}

bool NServer::Close(){
    return socket_.Close();
}

int NServer::Send(const std::vector<uint8_t> &data, const std::string ip_address) const {
    return socket_.Send(data, ip_address);
}

int NServer::Send(const std::vector<uint8_t> &data, const uint32_t ip_address) const {
    return socket_.Send(data, ip_address);
}

std::vector<uint8_t> NServer::WaitingMessage(const int sbuffer) const {
    return socket_.Receive(sbuffer);
}

int NServer::HandleMessage(const std::vector<uint8_t> &msg) const {
    std::vector<uint8_t> wrapped_msg(msg.begin()+(4*(msg.at(0) & 0x0F)),msg.end());
    uint32_t ip_from = (msg.at(12) << 24) + (msg.at(13) << 16) + (msg.at(14) << 8) + msg.at(15);

    EchoRequest echo_test;
    if ( echo_test.Decode(wrapped_msg) ) {
        EchoReply echo_response;
        echo_response.set_identifier(echo_test.identifier());
        echo_response.set_sequence_number(echo_test.sequence_number());
        echo_response.set_data(echo_test.data());

        return Send(echo_response.Encode(), ip_from);
    }
    return -1;
};