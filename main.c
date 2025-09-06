#include "main.h"

int main(int argc, const char** argv)
{
    if (argc != 2) {
        die("no port specified");
    }

    if (isnum(argv[1])) {
        const int PORT = atoi(argv[1]);

        print("%sop3l v0.0.0%s", COLOR_GREEN, COLOR_RESET);
        print("running on port http://localhost:%d", PORT);

        int server_socket;
        struct sockaddr_in server_addr;

        check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
            "failed to create socket");

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        check(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)),
            "bind failed");
        check(listen(server_socket, SERVER_BACKLOG),
            "listen failed!");

        while (true) {
            // listen for connections from client
            int client_socket = listen_for_connections(server_socket);

            // parse http headers
            size_t header_len;
            char* headers = read_headers(client_socket, &header_len);

            if (!headers) {
                close(client_socket);
                die("could not read request headers");
            }

#ifdef OUTPUT_VERBOSE
            printf("Headers: \"%s\"\n", headers);
#endif

            // size_t content_length = get_content_length(headers);
            // char* body = read_body(client_socket, content_length);
            // if (body == NULL) {
            //     free(headers);
            //     close(client_socket); // HTTP/1.1 would normally keep alive
            //     continue;
            // }
            // printf("body len – %zu\n", content_length);

            const char* route = get_route_accessed(headers);
#ifdef OUTPUT_VERBOSE
            printf("route accessed – \"%s\"\n", route);
#endif

            if (strstr(route, ".") != NULL || strcmp(route, "/") == 0) {
                // static file requested or index route
                serve_static_file(client_socket, route);
            } else {
                // possibly a dynamic route, we shall deal with that later
                serve_dynamic_op3l_file(client_socket, route);
            }

            close(client_socket); // HTTP/1.1 would normally keep alive

            // free(body);
            free(headers);
        }
    } else {
        const char* FILEPATH = argv[1];
        interpret_file(FILEPATH);
    }

    return 0;
}
