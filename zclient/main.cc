#include <iostream>
#include <typeinfo>
#include "include/nclient.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Parameters are missing " << std::endl;
        return -1;
    }

    std::cout << "Initializing first client..." << std::endl;
    NClient client1;

    if ( !client1.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Client 1 Socket: " << client1.sock_fd() << std::endl;

    std::cout << client1.Ping(argv[1], std::stoi(argv[2]));
}