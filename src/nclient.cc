/**
 * \file
 * \brief NClient class definition
 */
#include "include/nclient.h"

#include <chrono>

#include "include/echo_reply.h"
#include "include/echo_request.h"

bool NClient::Open(const std::string ip_address){
    if ( isOpen() ){
        return false;
    }

    return socket_.CreateSocket() && socket_.SetIP(ip_address);
}

bool NClient::isOpen() const {
    return socket_.sock_fd() != -1;
}

bool NClient::Close(){
    return socket_.Close();
}

int NClient::Send(const std::vector<uint8_t> &data, const std::string ip_address) const {
    return socket_.Send(data, ip_address);
}

std::vector<uint8_t> NClient::WaitingMessage(const int sbuffer) const {
    return socket_.Receive(sbuffer);
}

std::string NClient::Ping(const std::string ip_address, const uint16_t sequence_number) const {
    
    uint16_t identifier = getpid();
    EchoRequest new_echo_request;

    new_echo_request.set_identifier(identifier);

    new_echo_request.set_data({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});

    std::stringstream ss = {};
    new_echo_request.set_sequence_number(sequence_number);

    Send(new_echo_request.Encode(), ip_address);
    auto send_time = std::chrono::high_resolution_clock::now().time_since_epoch();

    std::vector<uint8_t> msg = WaitingMessage(1024);

    if ( !msg.empty() ) {
        auto recv_time = std::chrono::high_resolution_clock::now().time_since_epoch();
        long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - send_time).count();

        std::vector<uint8_t> wrapped_msg(msg.begin()+(4*(msg.at(0) & 0x0F)),msg.end());
        EchoReply new_echo_reply;
        if ( new_echo_reply.Decode(wrapped_msg) ) {
            ss << wrapped_msg.size() << " bytes from " << ip_address << ": icmp_seq=" << sequence_number << " ttl=" << (int)msg.at(8) << " time=" << elapsed_time << " ms" << std::endl;
        }
    } else {
        ss << " 0 bytes from " << ip_address << ": icmp_seq=" << sequence_number << " ttl=null"  << " time=Timeout" << std::endl;
    }

    return ss.str();
}