/**
 * \file
 * \brief Echo Reply IPv6 class definition
 */
#include "echo_ipv6.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_REPLY_IPV6_H_
#define PING_PROJECT_INCLUDE_ECHO_REPLY_IPV6_H_

/**
 * \brief Echo Reply IPv6 class
 */
class EchoReplyIPv6 : public EchoIPv6
{
    public:
     /**
      * \brief Constructor
      */
     EchoReplyIPv6() : EchoIPv6(129, 0) {};
};

#endif

/*
@startuml

class EchoReplyIPv6 {
    +EchoReplyIPv6()
}

EchoReplyIPv6 --|> EchoIPv6

@enduml
*/