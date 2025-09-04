#include "main.h"

ssize_t recv_exact(int sock, char* buf, size_t n)
{
    size_t total = 0;
    while (total < n) {
        ssize_t r = recv(sock, buf + total, n - total, 0);
        if (r <= 0)
            return r; // error or connection closed
        total += r;
    }
    return total;
}

char* read_headers(int socket, size_t* output_len)
{
    size_t bufsize = INIT_BUFSIZE;
    char* buf = malloc(bufsize);
    if (!buf) {
        die("malloc failed");
    }

    size_t total = 0;
    while (1) {
        // grow buf if full
        if (total == bufsize) {
            bufsize *= 2;
            char* tmp = realloc(buf, bufsize);
            if (!tmp) {
                free(buf);
                die("realloc failed");
            }
            buf = tmp;
        }

        ssize_t n = recv(socket, buf + total, 1, 0); // read 1 byte at a time
        if (n <= 0) {
            free(buf);
            return NULL;
        }
        total += n;

        if (total >= 4 && strncmp(buf + total - 4, "\r\n\r\n", 4) == 0) {
            break; // end of headers
        }
    }

    buf[total] = '\0';
    if (output_len)
        *output_len = total;
    return buf;
}

size_t get_content_length(const char* headers)
{
    const char* cl = strcasestr(headers, "Content-Length:");
    if (!cl)
        return 0;

    cl += strlen("Content-Length:");
    while (*cl && isspace(*cl))
        cl++;

    return strtoul(cl, NULL, 10);
}

const char* get_route_accessed(const char* headers)
{
    char* copy = strdup(headers);
    if (!copy)
        return NULL;

    char* line = strtok(copy, "\r\n"); // first line only
    if (!line) {
        free(copy);
        return NULL;
    }

    __attribute__((unused)) char* method = strtok(line, " ");
    char* path = strtok(NULL, " ");
    // char *version = strtok(NULL, " ");

    const char* result = path ? strdup(path) : "";
    free(copy);
    return result;
}

char* read_body(int client_socket, size_t content_length)
{
    if (content_length == 0) {
        return NULL;
    }
    char* body = malloc(content_length + 1);
    if (!body) {
        // free(headers);
        close(client_socket);
        die("malloc failed");
    }

    if (recv_exact(client_socket, body, content_length) <= 0) {
        free(body);
        // free(headers);
        close(client_socket);
        return NULL;
    }

    body[content_length] = '\0';
    // printf("Body (%zu bytes): \"%s\"\n", content_length, body);
    return body;
}
