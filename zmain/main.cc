#include <chrono>
#include <cstdlib>  
#include <fstream>
#include <iostream>
#include <string>
#include <thread>        
#include <vector>

#include "include/echo_reply.h"
#include "include/echo_request.h"
#include "include/nclient.h"
#include "include/nserver.h"
#include "include/nsocket.h"


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

void Server(){
    NServer server;
    std::cout << "THREAD SERVER:::> Starting server...  " << std::endl;

    if ( !server.Open("127.0.0.5") ){
        std::cout << "THREAD SERVER:::> Failed" << std::endl;
    } else {
        std::cout<< "THREAD SERVER:::> Server is open, sock_fd_: " << server.sock_fd() << std::endl;

        for (int i = 0; i < 10; i++) {
            std::cout << "THREAD SERVER:::> Waiting new message..." << std::endl;
            std::vector<uint8_t> msg = server.WaitingMessage(1024);
            
            if ( !msg.empty() ) {
                std::cout << "THREAD SERVER:::> Message arrived..." << std::endl;

                // Adding delay, loss of data or not sending answer
                    std::this_thread::sleep_for(std::chrono::milliseconds(10 + rand() % (30 + 1)));
                    
                    if ( i % 4 == 0 ){
                        msg.pop_back();
                    }

                    if ( i == 5 ){
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        std::cout << "THREAD SERVER:::> Response unsent" << std::endl;
                        continue;
                    }

                int send_bytes = server.HandleMessage(msg);

                std::cout << "THREAD SERVER:::> Response sent with " << send_bytes << " bytes." << std::endl;

            } else {
                std::cout << "THREAD SERVER:::> Timeout...  " << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }
}

void Client(){
    std::this_thread::sleep_for(std::chrono::seconds(1));

    NClient client;
    std::cout << "THREAD CLIENT:::> Starting client... " << std::endl;

    if ( !client.Open("127.0.0.3") ){
        std::cout << "THREAD CLIENT:::> Falhou" << std::endl;
    } else {
        std::cout<< "THREAD CLIENT:::> Client is open, sock_fd_: " << client.sock_fd() << std::endl;
        
        std::stringstream response = {};
        for (int i = 0; i < 10; i++)
        {
            std::cout<< "THREAD CLIENT:::> Starting new ping" << std::endl;
            std::string resp_temp = client.Ping("127.0.0.5", i);
            response << resp_temp;          
        }
        
        std::cout<< "THREAD CLIENT:::> Ping result" << std::endl << response.str();
    }
}

int main() {
    std::cout << "MAIN:::> Deactivating temporarily echo reply from kernel..." << std::endl;

    if ( !set_kernel_icmp_response(true) ){
        std::cout << "MAIN:::> Failed..." << std::endl;
        return 0;
    }

    std::cout << "MAIN:::> Initializing program with threads..." << std::endl;

    std::thread thread_servidor(Server);
    std::thread thread_cliente(Client);

    thread_servidor.join();
    thread_cliente.join();

    std::cout << "MAIN:::> Program ended." << std::endl;
    
    std::cout << "MAIN:::> Reactivating echo reply from kernel..." << std::endl;

    if ( !set_kernel_icmp_response(false) ){
        std::cout << "Failed..." << std::endl;
        return 0;
    }
    return 0;
}




