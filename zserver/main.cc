/**
 * \brief Main for client execution. It should be initialized with at least 2 arguments (ipv4 default) and with 3 arguments for ipv6.
 * zclient/client ip_address number_of_messages type_of_ip (optional)
 * 
 * Anything different from ipv6 in the third argument will be treated as ipv4
 * 
 * \param argc Number of input arguments
 * \param argv Pointer to arguments 
 */

#include "include/nserver.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <thread>        
#include <vector>

std::string version = "ipv4";

bool set_kernel_icmp_response(bool ignore_echo_requests) {
    std::string sysctl_path;

    if (version == "ipv6") {
        sysctl_path = "/proc/sys/net/ipv6/icmp/echo_ignore_all";
    } else {
        sysctl_path = "/proc/sys/net/ipv4/icmp_echo_ignore_all";
    }

    std::ofstream sysctl_file(sysctl_path);

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

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Parameters are missing " << std::endl;
        return -1;
    }

    if (argc >= 4) {
        version = argv[3];
    }

    std::cout << "Deactivating temporarily echo reply from kernel..." << std::endl;

    if ( !set_kernel_icmp_response(true) ){
        std::cout << "Failed..." << std::endl;
        return 0;
    }

    std::cout << "Initializing server..." << std::endl;

    

    NServer server(argv[1], version);

    if ( !server.isOpen() ){
        std::cout << "Opening failed" << std::endl;    
        return -1;
    }
    std::cout << "Server Socket: " << server.sock_fd() << std::endl;

    for (int i = 0; i < std::stoi(argv[2]); i++) {
        std::cout << "Waiting new message..." << std::endl;
        std::vector<uint8_t> msg = server.Receive(4096);

        if ( !msg.empty() ) {   
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if ( i == 6 ){
                    msg.pop_back();
                } else if ( i == 9 ){
                    std::cout << "Reply unsent" << std::endl;
                    continue;
                }
            std::string answer = server.HandleMessage(msg);
            std::cout << answer;
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