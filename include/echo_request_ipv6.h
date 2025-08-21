/**
 * \file
 * \brief Echo request IPv6 class definition
 */
#include "echo_ipv6.h"

#ifndef PING_PROJECT_INCLUDE_ECHO_REQUEST_IPV6_H_
#define PING_PROJECT_INCLUDE_ECHO_REQUEST_IPV6_H_

/**
 * \brief Echo request IPv6 class
 */
class EchoRequestIPv6 : public EchoIPv6
{
    public:
     /**
      * \brief Constructor
      */
     EchoRequestIPv6() : EchoIPv6(128, 0) {};
};

#endif

/*
@startuml

class EchoRequest {
    +EchoRequestIPv6()
}

EchoRequestIPv6 --|> EchoIPv6

@enduml
*/