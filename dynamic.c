#include "main.h"

void serve_dynamic_op3l_file(int client_socket, [[maybe_unused]] const char* route)
{

    // test the f*ckin interpreter here
    struct chunk chunk;
    init_chunk(&chunk);

    write_constant(&chunk, 1.2, 1);
    write_chunk(&chunk, OP_RETURN, 123);

    disassemble_chunk(&chunk, "test chunk");
    free_chunk(&chunk);

    char res[1 << 17];
    sprintf(res,
        "HTTP/1.1 200 OK\n" OP3L_SERVER_HEADER
        "Content-Type: text/plain\n"
        "\n"
        "<h1>from <code>dynamic.c</code></h1>");
    write(client_socket, res, strlen(res));
}
