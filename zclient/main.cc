#include <iostream>

#include "include/nclient.h"

int main() {
    std::cout << "Initializing first client..." << std::endl;
    NClient client1;

    if ( !client1.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Client 1 Socket: " << client1.sock_fd() << std::endl;

    std::cout << client1.Ping("127.0.0.5", 10);

    std::cout << std::endl << "Initializing second client..." << std::endl;
    NClient client2;

    if ( !client2.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Client 2 Socket: " << client2.sock_fd() << std::endl;

    std::cout << client2.Ping("8.8.8.8", 10);

    std::cout << "Program ended" << std::endl;
    return 0;
}