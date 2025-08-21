/**
 * \file
 * \brief NClient class definition
 */
#include "include/nclient.h"

#include <chrono>
#include <sstream>
#include <stdexcept>

#include "include/echo.h"
#include "include/echo_reply.h"
#include "include/echo_reply_ipv6.h"
#include "include/echo_request.h"
#include "include/echo_request_ipv6.h"


NClient::NClient(std::shared_ptr<NSocket> socket_ptr){
    if( socket_ptr != nullptr ) {
        socket_ = socket_ptr;
    }

    if ( !socket_->CreateSocket() ){
        throw std::runtime_error("Socket creation failed");
    }
}

NClient::NClient(std::string version){
    if (version == "ipv6") {
        socket_ = std::make_shared<NSocketIPv6>();
    } else {
        socket_ = std::make_shared<NSocketIPv4>();
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
    std::stringstream ss = {};
    uint16_t identifier = getpid();

    std::shared_ptr<Echo> new_echo_request = nullptr;
    
    if ( socket_->type() == "ipv4" ) {
        new_echo_request = std::make_shared<EchoRequest>();
    } else if ( socket_->type() == "ipv6" ) {
        new_echo_request = std::make_shared<EchoRequestIPv6>();
    } else {
        return ss.str();
    }

    std::string ip_source = socket_->GetSourceIP(ip_address);
    std::string ip_destiny = socket_->DecodeAddress(ip_address);

    if (ip_source == "" || ip_destiny == "") {
        return "";
    }
    new_echo_request->set_ip_source(ip_source);
    new_echo_request->set_ip_destiny(ip_destiny);

    new_echo_request->set_identifier(identifier);
    new_echo_request->set_data({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

    for (int i = 0; i < times; i++)
    {
        new_echo_request->set_sequence_number(i);
        std::vector<uint8_t> msg1 = new_echo_request->Encode();

        if ( socket_->Send(msg1, ip_address) == -1){
            return ss.str();
        }
        auto send_time = std::chrono::high_resolution_clock::now().time_since_epoch();

        while (true) {
            std::vector<uint8_t> msg = socket_->Receive(4096);
            if ( (socket_->last_ip_com() != "") && (socket_->last_ip_com() != new_echo_request->ip_destiny())){
                continue;
            }
            if ( !msg.empty() ) {
                auto recv_time = std::chrono::high_resolution_clock::now().time_since_epoch();
                long long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - send_time).count();

                std::unique_ptr<Echo> NewEcho = nullptr;
                if ( socket_->type() == "ipv4" ) {
                    NewEcho = std::make_unique<EchoRequest>();
                    if ( NewEcho->Decode(msg) && (NewEcho->checksum() == 0 ) && (new_echo_request->isEqual(*NewEcho) == 1) ) {
                        //Do nothing, just an echo of the sent message
                        continue;
                    }

                    NewEcho = std::make_unique<EchoReply>();
                    if ( NewEcho->Decode(msg) && (NewEcho->checksum() == 0 ) && (new_echo_request->isEqual(*NewEcho) == 2) ) {
                        ss << msg.size() << " bytes from " << socket_->last_ip_com() << ": icmp_seq=" << i << " time=" << elapsed_time << " ms" << std::endl;
                        break;
                    }
                } else if ( socket_->type() == "ipv6" ) {
                    NewEcho = std::make_unique<EchoRequestIPv6>();
                    NewEcho->set_ip_source(new_echo_request->ip_source());
                    NewEcho->set_ip_destiny(new_echo_request->ip_destiny());
                    if ( NewEcho->Decode(msg) && (NewEcho->checksum() == 0 ) && (new_echo_request->isEqual(*NewEcho) == 1) ) {
                        //Do nothing, just an echo of the sent message
                        continue;
                    }

                    NewEcho = std::make_unique<EchoReplyIPv6>();
                    NewEcho->set_ip_source(new_echo_request->ip_source());
                    NewEcho->set_ip_destiny(new_echo_request->ip_destiny());
                    if ( NewEcho->Decode(msg) && (NewEcho->checksum() == 0 ) && (new_echo_request->isEqual(*NewEcho) == 2) ) {
                        ss << msg.size() << " bytes from " << socket_->last_ip_com() << ": icmp_seq=" << i << " time=" << elapsed_time << " ms" << std::endl;
                        break;
                    }
                } else {
                    return "Socket is broken";
                }

                ss << msg.size() << " bytes from " << socket_->last_ip_com() << ": unidentified or broken message" << std::endl;
            } else {
                ss << "No response from " << new_echo_request->ip_destiny() << ": icmp_seq=" << i << " time=Timeout" << std::endl;
            }
            break;
        }
    }
    return ss.str();
}