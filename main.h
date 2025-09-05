#ifndef MAIN_H
#define MAIN_H

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

// server constants
#define INIT_BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define SERVER_NAME "op3l@v0.0.0"
#define OP3L_SERVER_HEADER "Server: " SERVER_NAME "\r\n"
#define STATIC_DIRECTORY_PATH "/Users/japroz/coding/programming/op3l/samples"

// utils.c
void die(char* fmt, ...);
void print(char* fmt, ...);
bool endswith(const char* str, char* suffix);

// server.c
int check(int exp, const char* msg);
int listen_for_connections(int server_socket);

// http-parse.c
const char* get_route_accessed(const char* headers);
size_t get_content_length(const char* headers);
char* read_headers(int socket, size_t* output_len);
ssize_t recv_exact(int sock, char* buf, size_t n);
char* read_body(int socket, size_t content_length);

// static.c
void serve_static_file(int client_socket, const char* route);

// dynamic.c
void serve_dynamic_op3l_file(int client_socket, const char* route);

#endif
