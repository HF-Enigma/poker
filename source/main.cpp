#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

const char PLAYER_NAME[] = "hf_enigma";

using std::cout;

struct input_args {
    in_addr_t server_ip;
    in_port_t server_port;
    in_addr_t player_ip;
    in_port_t player_port;
    int player_id;
};

bool parse_input_args(int argc, char **argv, input_args &args){
    if(argc != 6){
        return false;
    }

    // parse input arguments
    args.server_ip = inet_addr(argv[1]);
    args.server_port = htons(atoi(argv[2]));
    args.player_ip = inet_addr(argv[3]);
    args.player_port = htons(atoi(argv[4]));
    args.player_id = atoi(argv[5]);

    // TODO: validate input args
    return true;
}

bool create_connection(input_args &args, int &s){
    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        cout << "failed to creat socket\n";
        return false;
    }

    // socket reuse
    int reuse_addr = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr))){
        cout << "failed to set socket option SO_REUSEADDR\n";
        return false;
    }

    // bind
    struct sockaddr_in player_addr;
    memset(&player_addr, 0, sizeof(player_addr));
    player_addr.sin_family = AF_INET;
    player_addr.sin_addr.s_addr = args.player_ip;
    player_addr.sin_port = args.player_port;
    if(bind(s, (struct sockaddr*)&player_addr, sizeof(player_addr))){
        cout << "failed to bind socket\n";
        return false;
    }

    // connect
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = args.server_ip;
    server_addr.sin_port = args.server_port;
    // keeps trying
    while(connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        // interval 100ms
        usleep(100000);
    }

    return true;
}

void communicate(int s, int player_id){
    // register
    char reg_msg[50];
    snprintf(reg_msg, sizeof(reg_msg) - 1, "reg: %d %s \n", player_id, PLAYER_NAME);
    send(s, reg_msg, strlen(reg_msg) + 1, 0);

    // game loop
    char buffer[1024];
    while(true){
        int length = recv(s, buffer, sizeof(buffer), 0);
        if(length > 0){
            /* on_server_message返回-1（比如收到game over消息），则跳出循环，关闭socket，安全退出程序 */
//            if (-1 == on_server_message(length, buffer)){
//                break;
//            }
        }
    }

    // close
    close(s);
}

int main(int argc, char **argv){
    // parse input arguments
    input_args args;

    if(!parse_input_args(argc, argv, args)){
        cout << "usage: game <server_ip> <server_port> <player_ip> <player_port> <player_id>\n";
        return -1;
    }

    // connect to server
    int client_socket;

    if(!create_connection(args, client_socket)){
        return -1;
    }

    return 0;
}
