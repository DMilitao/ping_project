/**
 * \file
 * \brief NServer class definition
 */
#include "include/nserver.h"

#include <string>
#include <sstream>

#include "include/echo_reply.h"
#include "include/echo_reply_ipv6.h"
#include "include/echo_request.h"
#include "include/echo_request_ipv6.h"

NServer::NServer(const std::string ip_address, std::shared_ptr<NSocket> socket_ptr){
    socket_ = socket_ptr;

    if ( !socket_->CreateSocket() ){
        throw std::runtime_error("Socket creation failed");
    }

    if ( !socket_->SetIP(ip_address) ){
        throw std::runtime_error("IP definition failed");
    }
    ip_server_ = ip_address;
}

NServer::NServer(const std::string ip_address, std::string version){
    if (version == "ipv6") {
        socket_ = std::make_shared<NSocketIPv6>();
    } else {
        socket_ = std::make_shared<NSocketIPv4>();
    }

    if ( !socket_->CreateSocket() ){
        throw std::runtime_error("Socket creation failed");
    }

    if ( !socket_->SetIP(ip_address) ){
        throw std::runtime_error("IP definition failed");
    }
    ip_server_ = ip_address;
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
    std::shared_ptr<Echo> new_echo_request = nullptr;

    if ( socket_->type() == "ipv4" ) {
        new_echo_request = std::make_shared<EchoRequest>();
    } else if ( socket_->type() == "ipv6" ) {
        new_echo_request = std::make_shared<EchoRequestIPv6>();
    } else {
        return ss.str();
    }

    new_echo_request->set_ip_source(socket_->last_ip_com());
    new_echo_request->set_ip_destiny(socket_->DecodeAddress(ip_server_));

    if ( new_echo_request->Decode(msg) && (new_echo_request->checksum() == 0 )) {
        std::shared_ptr<Echo> new_echo_reply = nullptr;
        if ( socket_->type() == "ipv4" ) {
            new_echo_reply = std::make_shared<EchoReply>();
        } else if ( socket_->type() == "ipv6" ) {
            new_echo_reply = std::make_shared<EchoReplyIPv6>();
        } else {
            return ss.str();
        }

        new_echo_reply->set_ip_source(new_echo_request->ip_destiny());
        new_echo_reply->set_ip_destiny(new_echo_request->ip_source());

        new_echo_reply->set_identifier(new_echo_request->identifier());
        new_echo_reply->set_sequence_number(new_echo_request->sequence_number());
        new_echo_reply->set_data(new_echo_request->data());

        int bytes_sent = socket_->Send(new_echo_reply->Encode(), socket_->last_ip_com());

        if ( bytes_sent != -1 ){
           ss << "EchoRequest received from " << socket_->last_ip_com() << ": reply sent with " << bytes_sent << " bytes" << std::endl;
        } else {
           ss << "EchoRequest received from " << socket_->last_ip_com() << ": error sending response" << std::endl;
        }
    } else {
        ss << "Unidentified message received from " << socket_->last_ip_com() << std::endl;
    }
    return ss.str();
}