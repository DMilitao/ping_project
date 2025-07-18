#include <iostream>

#include "include/echo_reply.h"
#include "include/echo_request.h"
#include "include/nclient.h"
#include "include/nserver.h"
#include "include/nsocket.h"

int main(){
    EchoRequest new_echo_msg;

    std::cout << "Estou rodando aqui::::  " << std::endl;

    NServer server;
    if ( !server.StartServer(8081, 0x7F000001) ){
        std::cout << "Falha en criação do servidor"  << std::endl;
        return -1;
    }

    return 0;
    std::unique_ptr<NSocket> new_client = std::make_unique<NSocket>();

    std::cout << "Conecteeeeei::::  " << std::endl;
    
    server.AcceptConnection();

    std::cout << "Conecteeeeei::::  " << std::endl;

    std::cout << "Client sock:  " << new_client->sock_fd() << std::endl;
    std::cout << "Client IP:  " << std::hex << new_client->address().sin_addr.s_addr << std::endl;

    std::cout << "Trava aqui::::  " << std::endl;
return 0;
}



