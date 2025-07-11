#include "include/icmp.h"

#include <stdint.h>

#include <iostream>
#include <vector>

bool icmp::Decode(std::vector<uint8_t> buffer){
    std::size_t sbuffer = buffer.size();

    if (sbuffer < 8 ) { return false; } // Minimal size of a message to be implemented after

     set_Type(buffer[0]);
     set_Code(buffer[1]);

     if ( isEchoRequest() ){
        std::cout << "Is Echo Request" << std::endl;
        return true;
     } else if ( isEchoReply() ) {
        std::cout << "Is Echo Reply" << std::endl;
        return true;
     } else if ( isTimeStamp() ) {
        std::cout << "Is TimeStamp" << std::endl;
        return true;
     } else if ( isTimeStampReply() ) {
        std::cout << "Is TimeStamp Reply" << std::endl;
        return true;
     } else if ( isInfoRequest() ) {
        std::cout << "Is Info Request" << std::endl;
        return true;
     } else if ( isInfoReply() ) {
        std::cout << "Is Info Reply" << std::endl;
        return true;
     } else if ( isDestinationUnreach() ) {
        std::cout << "Is Dest Unreach" << std::endl;
        return true;
     } else if ( isRedirectMessage() ) {
        std::cout << "Is Redirect Mess" << std::endl;
        return true;
     } else if ( isSourceQuench() ) {
        std::cout << "Is Source Quench" << std::endl;
        return true;
     } else if ( isTimeExceed()) {
        std::cout << "Is Time Exceed" << std::endl;
        return true;
     } else if ( isParameterProblem() ) {
        std::cout << "Is Parameter Problem" << std::endl;
        return true;
     }

    return false;
}