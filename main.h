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
int isnum(const char* str);

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

// OP3L FILE PARSER/INTERPRETER STUFF
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STACK_MAX 256

void interpret_file(const char* file_path);

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG, // 24 bit wide
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN
} opcode;

typedef double OP3L_VALUE;

struct value_array {
    int capacity;
    int count;
    OP3L_VALUE* values;
};

struct chunk {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    struct value_array constants;
};

void init_chunk(struct chunk* chunk);
void free_chunk(struct chunk* chunk);
void write_chunk(struct chunk* chunk, uint8_t byte, int line);
int add_constant(struct chunk* chunk, OP3L_VALUE value);
void write_constant(struct chunk* chunk, OP3L_VALUE value, int line);
void* reallocate(void* pointer, size_t old_size, size_t new_size);

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count)    \
    (type*)reallocate(pointer, sizeof(type) * (old_count), \
        sizeof(type) * (new_count))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void disassemble_chunk(struct chunk* chunk, const char* name);
int disassemble_instr(struct chunk* chunk, int offset);

void print_value(OP3L_VALUE value);
void init_value_array(struct value_array* array);
void write_value_array(struct value_array* array, OP3L_VALUE value);
void free_value_array(struct value_array* array);

struct vm {
    struct chunk* chunk;
    uint8_t* ip;
    OP3L_VALUE stack[STACK_MAX];
    OP3L_VALUE* stack_top;
};

enum interpret_result {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

void init_vm(void);
void free_vm(void);
enum interpret_result interpret(struct chunk* chunk);
void push(OP3L_VALUE value);
OP3L_VALUE pop();

#endif
