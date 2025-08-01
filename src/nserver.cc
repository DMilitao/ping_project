/**
 * \file
 * \brief NServer class definition
 */
#include "include/nserver.h"

#include <string>
#include <sstream>

#include "include/echo_request.h"
#include "include/echo_reply.h"

NServer::NServer(const std::string ip_address, std::shared_ptr<NSocket> socket_ptr){
    if( socket_ptr != nullptr ) {
        socket_ = socket_ptr;
    } else {
        socket_ = std::make_shared<NSocket>();
    }

    if ( !socket_->CreateSocket() ){
        throw std::runtime_error("Socket creation failed");
    }

    if ( !socket_->SetIP(ip_address) ){
        throw std::runtime_error("IP definition failed");
    }
}

NServer::~NServer(){
    if ( !socket_->Close() ){
        throw std::runtime_error("Socket deletion failed");
    }
}

bool NServer::isOpen() const {
    return socket_->sock_fd() != -1;
}

std::vector<uint8_t> NServer::Receive(const int sbuffer) const {
    return socket_->Receive(sbuffer);
}

std::string NServer::HandleMessage(const std::vector<uint8_t> &msg) const {
    std::stringstream ss;
    ss << std::dec << (int)msg.at(12) << "." << (int)msg.at(13) << "." << (int)msg.at(14) << "." << (int)msg.at(15);
    std::string ip_from = ss.str();
    ss.str("");

    EchoRequest echo_test;
    if ( echo_test.Decode(msg) ) {
        EchoReply echo_response;
        echo_response.set_identifier(echo_test.identifier());
        echo_response.set_sequence_number(echo_test.sequence_number());
        echo_response.set_data(echo_test.data());

        int bytes_sent = socket_->Send(echo_response.Encode(), ip_from);
        ss << "EchoRequest received from " << ip_from << ": response sent with " << bytes_sent << " bytes" << std::endl;
    }
    return ss.str();
}