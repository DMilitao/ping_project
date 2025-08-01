#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>        
#include <vector>

#include "include/nserver.h"

bool set_kernel_icmp_response(bool ignore_echo_requests) {
    std::ofstream sysctl_file("/proc/sys/net/ipv4/icmp_echo_ignore_all");
    if (!sysctl_file.is_open()) {
        return false;
    }

    if (ignore_echo_requests) {
        sysctl_file << "1";
    } else {
        sysctl_file << "0";
    }
    sysctl_file.close();
    return true;
}

int main() {
    std::cout << "Deactivating temporarily echo reply from kernel..." << std::endl;

    if ( !set_kernel_icmp_response(true) ){
        std::cout << "Failed..." << std::endl;
        return 0;
    }

    std::cout << "Initializing server..." << std::endl;

    NServer server("127.0.0.5");

    if ( !server.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Server Socket: " << server.sock_fd() << std::endl;

    for (int i = 0; i < 13; i++) {
        std::cout << "Waiting new message..." << std::endl;
        std::vector<uint8_t> msg = server.Receive(1024);

        if ( !msg.empty() ) {
            // Adding delay, loss of data or not sending answer
                std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % (50 + 1)));
                if ( i == 6 ){
                    msg.pop_back();
                } else if ( i == 9 ){
                    std::cout << "Response unsent" << std::endl;
                    continue;
                }

            std::cout << server.HandleMessage(msg);
        } else {
            std::cout << "Timeout...  " << std::endl;
        }
    }

    std::cout << "Program ended." << std::endl;

    std::cout << "Reactivating echo reply from kernel..." << std::endl;

    if ( !set_kernel_icmp_response(false) ){
        std::cout << "Failed..." << std::endl;
        return 0;
    }
    return 0;
}