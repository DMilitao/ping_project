#include <iostream>

#include "include/echo_reply.h"
#include "include/echo_request.h"
#include "include/nclient.h"
#include "include/nserver.h"

int main(){
    EchoRequest new_echo_msg;

    uint8_t type = 8;
    uint8_t code = 0;
    std::vector<uint8_t> data = {1, 2, 3, 4};

    uint16_t identifier = 0xABCD;
    uint16_t sequence_number = 0xCDEF;

    new_echo_msg.set_type(type);
    new_echo_msg.set_code(code);
    new_echo_msg.set_data(data);
    new_echo_msg.set_identifier(identifier);
    new_echo_msg.set_sequence_number(sequence_number);

    std::vector<uint8_t> message = {1, 2, 3, 4};
    
    std::cout << "Estou rodando aqui::::  " << std::endl;

    NClient client;

    if ( !client.ConnectToServer(8080, 0x7F000001) ){
        return -1;
    }
    bool flag = client.Send(message);

    std::cout << "Enviei mensagem::::  " << std::endl;   
return 0;
}



