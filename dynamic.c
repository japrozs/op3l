#include "main.h"

void serve_dynamic_op3l_file(int client_socket, const char* route)
{
    char res[1 << 17];
    sprintf(res,
        "HTTP/1.1 200 OK\n" OP3L_SERVER_HEADER
        "Content-Type: text/plain\n"
        "\n"
        "<h1>from <code>dynamic.c</code></h1>");
    write(client_socket, res, strlen(res));
}