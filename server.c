#include "main.h"

int check(int exp, const char* msg)
{
    if (exp == SOCKETERROR) {
        fprintf(stderr, "%s[-]%s %s: ", COLOR_RED, COLOR_RESET, msg);
        perror("");
        exit(EXIT_FAILURE);
    }

    return exp;
}

int listen_for_connections(int server_socket)
{
    int client_socket, addr_size;
    struct sockaddr_in client_addr;
    print("actively listening for connections... \n");
    check(client_socket = (accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size)),
        "accept failed!");
    return client_socket;
}
