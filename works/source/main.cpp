#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "engine/game.h"

const char PLAYER_NAME[] = "hf_enigma";

using std::cout;

struct InputArgs {
    in_addr_t server_ip;
    in_port_t server_port;
    in_addr_t player_ip;
    in_port_t player_port;
    int player_id;
};

bool parseInputArgs(int argc, char **argv, InputArgs &args){
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

bool createConnection(InputArgs &args, int &s){
    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == -1){
        perror("failed to creat socket");
        return false;
    }

    // socket reuse
    int reuse_addr = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) == -1){
        perror("failed to set socket option SO_REUSEADDR");
        return false;
    }

    // bind
    struct sockaddr_in player_addr;
    memset(&player_addr, 0, sizeof(player_addr));
    player_addr.sin_family = AF_INET;
    player_addr.sin_addr.s_addr = args.player_ip;
    player_addr.sin_port = args.player_port;
    if(bind(s, (struct sockaddr*)&player_addr, sizeof(player_addr)) == -1){
        perror("failed to bind socket");
        return false;
    }

    // connect
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = args.server_ip;
    server_addr.sin_port = args.server_port;
    // keeps trying
    while(connect(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        // interval 100ms
        usleep(100000);
    }

    return true;
}

void communicate(int s, int player_id){
    // register
    Game *g = new Game(s, 8);
    g->sendRegMsg(player_id, PLAYER_NAME, true);

    // game loop
    char *buffer = new char[DEFAULT_BUFFER_SIZE/2];
    while(true){
        int size = recv(s, buffer, DEFAULT_BUFFER_SIZE/2, 0);
        if(size > 0){
            if (g->onMsg(buffer, size) == -1){
                break;
            }
        }
        else if(size == -1){
            perror("failed to recv packet");
        }
    }

    delete[] buffer;
    delete g;
}

int main(int argc, char **argv){
    // parse input arguments
    InputArgs args;
    if(!parseInputArgs(argc, argv, args)){
        printf("usage: game <server_ip> <server_port> <player_ip> <player_port> <player_id>\n");
        return -1;
    }

    // connect to server
    int client_socket;
    if(!createConnection(args, client_socket)){
        return -1;
    }

    // communicate with server
    communicate(client_socket, args.player_id);

    return 0;
}
