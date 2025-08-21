/**
 * \brief Main for client execution. It should be initialized with at least 2 arguments (ipv4 default) and with 3 arguments for ipv6.
 * zclient/client ip_address number_of_messages type_of_ip (optional)
 *
 * \param argc Number of input arguments
 * \param argv Pointer to arguments 
 */

#include "include/nclient.h"

#include <iostream>

std::string version = "ipv4";

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Parameters are missing " << std::endl;
        return -1;
    }

    std::cout << "Initializing first client..." << std::endl;

    if (argc >= 4) {
        version = argv[3];
    }

    NClient client1(version);

    if ( !client1.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Client 1 Socket: " << client1.sock_fd() << std::endl;

    std::cout << client1.Ping(argv[1], std::stoi(argv[2]));

    return 0;
}