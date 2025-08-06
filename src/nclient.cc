/**
 * \file
 * \brief NClient class definition
 */
#include "include/nclient.h"

#include <chrono>
#include <sstream>
#include <stdexcept>

#include "include/echo_reply.h"
#include "include/echo_request.h"

NClient::NClient(std::shared_ptr<NSocket> socket_ptr){
    if( socket_ptr != nullptr ) {
        socket_ = socket_ptr;
    } else {
        socket_ = std::make_shared<NSocket>();
    }

    if ( !socket_->CreateSocket() ){
        throw std::runtime_error("Socket creation failed");
    }
}

NClient::~NClient(){
    if ( !socket_->Close() ){
        throw std::runtime_error("Socket deletion failed");
    }
}

bool NClient::isOpen() const {
    if (socket_ != nullptr) {
        return socket_->sock_fd() != -1;
    }
    return false;
}

std::string NClient::Ping(const std::string ip_address, const int times) const {
    uint16_t identifier = getpid();
    EchoRequest new_echo_request;

    new_echo_request.set_identifier(identifier);

    new_echo_request.set_data({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

    std::stringstream ss = {};

    for (int i = 0; i < times; i++)
    {
        new_echo_request.set_sequence_number(i);

        if ( socket_->Send(new_echo_request.Encode(), ip_address) == -1){
            return ss.str();
        }
        auto send_time = std::chrono::high_resolution_clock::now().time_since_epoch();

        while (true) {
            std::vector<uint8_t> msg = socket_->Receive(1024);
            if ( !msg.empty() ) {
                auto recv_time = std::chrono::high_resolution_clock::now().time_since_epoch();
                long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - send_time).count();

                EchoRequest new_echo_request_echo;
                EchoReply new_echo_reply;
                if ( new_echo_request_echo.Decode(msg) && (new_echo_request_echo.checksum() == 0 ) && (new_echo_request_echo == new_echo_request) ) {
                    //Do nothing, just an echo of the sent message
                } else if ( new_echo_reply.Decode(msg) && (new_echo_reply.checksum() == 0 ) && ( new_echo_request == new_echo_reply ) ) {
                    ss << msg.size() << " bytes from " << ip_address << ": icmp_seq=" << i << " ttl=" << (int)msg.at(8) << " time=" << elapsed_time << " ms" << std::endl;
                    break;
                } else {
                    ss << "00 bytes from " << ip_address << ": icmp_seq=" << i << " ttl=null"  << " time=Timeout" << std::endl;
                    break;
                }
            } else {
                ss << "00 bytes from " << ip_address << ": icmp_seq=" << i << " ttl=null"  << " time=Timeout" << std::endl;
                break;
            }
        }
    }
    return ss.str();
}