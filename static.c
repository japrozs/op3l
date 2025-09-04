#include "main.h"

const char* get_mime_type(const char* path)
{
    if (strstr(path, ".html"))
        return "text/html";
    if (strstr(path, ".css"))
        return "text/css";
    if (strstr(path, ".js"))
        return "application/javascript";
    if (strstr(path, ".png"))
        return "image/png";
    if (strstr(path, ".jpg"))
        return "image/jpeg";
    return "text/plain";
}

void serve_static_file(int client_socket, const char* route)
{
    char filepath[1024];
    if (strcmp(route, "/") == 0) {
        snprintf(filepath, sizeof(filepath), "%s/index.html", STATIC_DIRECTORY_PATH);
    } else {
        snprintf(filepath, sizeof(filepath), "%s%s", STATIC_DIRECTORY_PATH, route);
    }

    print("serving file  – \"%s\"", filepath);
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        const char* notfound = "HTTP/1.1 404 Not Found\r\n" OP3L_SERVER_HEADER
                               "Content-Type: text/plain\r\n"
                               "Content-Length: 13\r\n"
                               "\r\n"
                               "404 Not Found";
        write(client_socket, notfound, strlen(notfound));
        return;
    }

    struct stat st;
    stat(filepath, &st);
    long fsize = st.st_size;

    char* buf = malloc(fsize);
    fread(buf, 1, fsize, f);
    fclose(f);

    // send headers
    const char* mime = get_mime_type(filepath);
    char header[1024];
    int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n" OP3L_SERVER_HEADER
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        mime, fsize);

    write(client_socket, header, hlen);
    write(client_socket, buf, fsize);

    free(buf);
}
